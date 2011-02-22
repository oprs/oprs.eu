
(declaim (optimize (speed 0) (safety 3) (debug 3)))

(declaim (special *derivatives*))

; helpers

(defun stream-pull (stream)
  (read-char stream nil))

(defun stream-peek (stream)
  (peek-char nil stream nil))

; type checks

(defun alt? (atom)
  (and (consp atom) (eq (car atom) 'alt)))

(defun rep? (atom)
  (and (consp atom) (eq (car atom) 'rep)))

(defun seq? (atom)
  (and (consp atom) (eq (car atom) 'seq)))

(defun any? (atom)
  (and (symbolp atom) (eq atom 'any)))

(defun empty? (atom)
  (eq atom nil))

(defun epsilon? (atom)
  (eq atom t))

(defun nullable? (atom)
  (or
    (epsilon? atom)
    (rep? atom)
    (and (alt? atom) (or  (nullable? (cadr atom)) (nullable? (caddr atom))))
    (and (seq? atom) (and (nullable? (cadr atom)) (nullable? (caddr atom))))))

(defun equiv? (latom ratom)
;(format t "comparing: ~a <-> ~a~%" latom ratom)
  (or
    (equal latom ratom)
    (and (alt? latom) (equal (alt-swap latom) ratom))
    (and (alt? ratom) (equal (alt-swap ratom) latom))))

(defun update-derivatives (atom)
  (or (position atom *derivatives* :test #'equiv?)
      (- (length (setq *derivatives* (append *derivatives* (cons atom nil)))) 1)))

(defun alt (latom ratom)
  (cond
    ((empty? latom) ratom)
    ((empty? ratom) latom)
;    ((equiv? latom ratom) latom)           ; XXX unclear why not
    ((equiv? latom ratom) (cons latom nil)) ; XXX unclear why
    (t (list 'alt latom ratom))))

(defun seq (latom ratom)
  (cond
    ((empty?   latom) nil)
    ((empty?   ratom) nil)
    ((epsilon? latom) ratom)
    ((epsilon? ratom) latom)
    (t (list 'seq latom ratom))))

(defun rep (atom)
  (cond
    ((empty?   atom) t)
    ((epsilon? atom) t)
    (t (list 'rep atom))))


(defun alt-swap (atom)
    (alt (caddr atom) (cadr atom)))


(defun re-deriv (atom sym)
  (cond
    ((rep? atom) (seq (re-deriv (cadr atom) sym) (rep (cadr atom))))
    ((alt? atom) (alt (re-deriv (cadr atom) sym) (re-deriv (caddr atom) sym)))
    ((seq? atom) (alt
      (seq (re-deriv (cadr atom) sym) (caddr atom))
      (seq (nullable? (cadr atom)) (re-deriv (caddr atom) sym))))
    (t (or (any? atom) (eq sym atom)))))


(defun re-format (atom)
  (cond
    ((rep? atom) (concatenate 'string "(" (re-format (cadr atom)) "*)"))
    ((alt? atom) (if (epsilon? (caddr atom))
                     (concatenate 'string "(" (re-format (cadr atom)) "?)")
                     (concatenate 'string "(" (re-format (cadr atom)) "|" (re-format (caddr atom)) ")")))
    ((seq? atom) (if (equiv? (rep (cadr atom)) (caddr atom))
                     (concatenate 'string "(" (re-format (cadr atom)) "+)")
                     (concatenate 'string (re-format (cadr atom)) (re-format (caddr atom)))))
    ((any? atom) (cons #\. nil))
    (t (cons atom nil))))


(defun re-atom (atom)
  (if (cdr atom) atom (car atom)))


(defun re-multiple-alt (l)
  (when (car l)
    (if (cdr l)
      (alt (car l) (re-multiple-alt (cdr l)))
      (car l))))


(defun re-quoted (stream)
  (let ((c (stream-pull stream)))
    (cond
      ((eq c #\0) (code-char #x00)) ; #\Null
      ((eq c #\a) (code-char #x07)) ; #\Bell
      ((eq c #\b) (code-char #x08)) ; #\Backspace
      ((eq c #\t) (code-char #x09)) ; #\Tab
      ((eq c #\n) (code-char #x0a)) ; #\Newline
      ((eq c #\v) (code-char #x0b)) ; #\Vt
      ((eq c #\f) (code-char #x0c)) ; #\Page
      ((eq c #\r) (code-char #x0d)) ; #\Return
      (t c))))


(defun re-range (lo hi)
  (do
    ((atom nil)
     (c (char-code lo) (1+ c)))
    ((> c (char-code hi)) (re-atom atom))
    (push (code-char c) atom)))


(defun re-charset (stream)
  (let* ((atom nil)
         (c (stream-pull stream))
         (n (eq c #\^)))
    (when n
      (setq c (stream-pull stream)))
    (loop
      (cond
        ((null c)   (return "eof")) ; XXX
        ((eq c #\\) (push (re-quoted) a))
        ((eq c #\-)
          (setq c (stream-pull stream))
; (if (null c)) ...
          (setq atom (union (re-range (pop atom) c) atom)))
        ((eq c #\]) (return (re-multiple-alt (reverse atom))))
        (t (push c atom)))
      (setq c (stream-pull stream)))))


(defun re-closure (stream atom)
  (let ((c (stream-peek stream)))
    (cond
      ((eq c #\?) (stream-pull stream) (alt atom t))
      ((eq c #\*) (stream-pull stream) (rep atom))
      ((eq c #\+) (stream-pull stream) (seq atom (rep atom)))
      (t atom))))


(defun re-expr (stream)
  (let ((atom nil)
        (c (stream-pull stream)))
    (unless (or (null c) (eq c #\)))
      ; first character
      (setq atom
        (cond
          ((eq c #\() (re-closure stream (re-expr    stream)))
          ((eq c #\[) (re-closure stream (re-charset stream)))
          ((eq c #\\) (cons (re-quoted stream) nil)) ; XXX closure
          ((eq c #\.) (cons 'any nil))               ; XXX closure
          (t          (cons c nil))))                ; XXX closure
      ; subsequent characters
      (do ((c (stream-pull stream) (stream-pull stream)))
        ((or (null c) (eq c #\))) (re-atom atom))
        (setq atom
          (cond
            ((eq c #\|) (alt (re-atom atom) (re-closure stream (re-expr    stream))))
            ((eq c #\() (seq (re-atom atom) (re-closure stream (re-expr    stream))))
            ((eq c #\[) (seq (re-atom atom) (re-closure stream (re-charset stream))))
            ((eq c #\\) (seq (re-atom atom) (re-closure stream (re-quoted  stream))))
            ((eq c #\.) (seq (re-atom atom) (re-closure stream 'any)))
            (t          (seq (re-atom atom) (re-closure stream c)))))))))


(defun re-match (re pattern)
  (let ((d re))
    (format t "       ~a~%" (re-format d) )
    (with-input-from-string (stream pattern)
      (do ((c (stream-pull stream) (stream-pull stream)))
          ((null c) "done")
          (setq d (re-deriv d c))
          (format t "~@c -> ~a~%" c (re-format d)))
      d)))

(defun re-map (atom alphabet)
  (when atom
    (format t "mapping ~s over ~s~%" atom alphabet)
    (or (epsilon? atom)
        (loop for x in alphabet
              collect (let* ((d (re-deriv atom x))
                             (n (update-derivatives d)))
                        (format t " lookup ~s: ~a -> ~a~%" x (re-format d) n)
                        (if (consp atom)
                            (re-map d alphabet)
                            n))))))
;                            (eq atom x)))))))

(defun re-compile (atom)
  (let ((*derivatives* '(nil t)))
(format t "~a"
  (re-map atom '( #\a #\b #\c #\d #\e ))))
;  (re-map atom '( #\a #\b #\c #\d #\e #\f #\g #\h #\i #\j #\k #\l #\m #\n #\o #\p #\q #\r #\s #\t #\u #\v #\w #\x #\y #\z ))))
)


(defun regex (re)
  (with-input-from-string (stream re)
    (re-expr stream)))

(regex "to.?o")
(re-format (regex "to.?o"))

(setq re (regex "(a|(bcde*)d)+"))
(setq dr (re-deriv re #\b))
(re-format re)
(re-format dr)

(setq re (regex "a(bc*)"))
(setq dr (re-deriv re #\a))
(re-format re)
(re-format dr)

(setq re (regex "a|b|b|a"))
(alt-swap re)

(re-match (regex "toto|((t|o).(t|o)(t|o))") "titt")

(setq re (regex "(a.)+|b(cd)?"))
(format t "~%\"~a\": ~a~%" (re-format re) re)
;(re-compile re)

;(setq re (regex "(a)?bc(d)?"))
(setq re (regex "(ab|ac)d"))
(format t "~%\"~a\": ~a~%" (re-format re) re)
(re-compile re)

