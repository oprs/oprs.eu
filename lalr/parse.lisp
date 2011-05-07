
(defparameter *lexemes* (make-hash-table))
(defparameter *tokens*  (make-hash-table))

(defmacro lexeme (label atom)
 `(setf (gethash ',label *lexemes*) ',atom))

(defmacro token (label atom)
 `(setf (gethash ',label *tokens*) ',atom))


(defun is-seq     (atom) (and (consp atom) (eq (car atom) 'seq)))
(defun is-alt     (atom) (and (consp atom) (eq (car atom) 'alt)))
(defun is-rep     (atom) (and (consp atom) (eq (car atom) 'rep)))
(defun is-opt     (atom) (and (consp atom) (eq (car atom) 'opt)))
(defun is-empty   (atom) (null atom))
(defun is-epsilon (atom) (eq atom t))
(defun is-any     (atom) (eq atom 'any))

(defun expand-seq (atom)
  (cond
    ((>  (length atom) 2) (list 'seq (expand-tok (car atom)) (expand-seq (cdr  atom))))
    ((eq (length atom) 2) (list 'seq (expand-tok (car atom)) (expand-tok (cadr atom))))
    ((eq (length atom) 1) (expand-tok (cadr atom)))
    (t (error "malformed sequence: ~s~%" atom))))

(defun expand-alt (atom)
  (cond
    ((>  (length atom) 2) (list 'alt (expand-tok (car atom)) (expand-alt (cdr  atom))))
    ((eq (length atom) 2) (list 'alt (expand-tok (car atom)) (expand-tok (cadr atom))))
    (t (error "malformed sequence: ~s~%" atom))))

(defun expand-str (atom)
  (expand-seq (concatenate 'list atom)))

(defun expand-lex (atom)
  (let ((lexeme (gethash atom *lexemes*)))
    (if lexeme (expand-tok lexeme)
               (error "unknown lexeme ~s~%" atom))))

(defun expand-tok (atom)
;(format t "expanding ~s~%" atom)
  (cond
    ((is-seq     atom) (expand-seq (cdr atom)))
    ((is-alt     atom) (expand-alt (cdr atom)))
    ((is-rep     atom) (list 'rep (expand-tok (cadr atom))))
    ((is-opt     atom) (list 'opt (expand-tok (cadr atom))))
    ((stringp    atom) (expand-str atom))
    ((symbolp    atom) (expand-lex atom))
    ((characterp atom) atom)
    (t (error "unsupported token type: ~s~%" atom))))

(defun expand (atom)
  (let ((token (gethash atom *tokens*)))
    (if token (expand-tok token)
              (expand-lex atom))))


(load "c-tokens")
(format t "~s~%" *tokens*)

