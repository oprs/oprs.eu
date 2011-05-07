
(defparameter *symbols* (make-hash-table))

(defmacro define (label args)
 `(setf (gethash ',label *symbols*) ',args))

(load (merge-pathnames "c-grammar" *load-truename*))

(defun dump-symbol (atom)
  (let ((val (gethash atom *symbols*)))
;(format t "~s: ~s~%" atom val)
      (if (null val)
        (error "undefined lexem ~s" atom)
        (dump val))))

(defun dump (atom)
  (cond
    ((symbolp atom) (dump-symbol atom))
    ((consp   atom) (nconc (cons (car atom) nil)
                           (mapcar #'dump (cdr atom))))
    (t        atom)))

