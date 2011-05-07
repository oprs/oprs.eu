
(defparameter *tokens* (make-hash-table))

(defmacro token (label args)
 `(setf (gethash ',label *tokens*) ',args))

;(load (merge-pathnames "c-grammar" *load-truename*))

(defun dump-token (atom)
  (let ((val (gethash atom *tokens*)))
;(format t "~s: ~s~%" atom val)
      (if (null val)
        (error "undefined lexem ~s" atom)
        (dump val))))

(defun dump (atom)
  (cond
    ((symbolp atom) (dump-token atom))
    ((consp   atom) (nconc (cons (car atom) nil)
                           (mapcar #'dump (cdr atom))))
    (t        atom)))

