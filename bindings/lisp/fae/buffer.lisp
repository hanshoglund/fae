(in-package :cl-user)
(define-foreign-type buffer-type () () (:actual-type :pointer))
(define-parse-method buffer () (make-instance 'buffer-type))
(defclass buffer () ((buffer-ptr :initarg :buffer-ptr)))
(defmethod translate-to-foreign (x (type buffer-type)) (slot-value x 'buffer-ptr))
(defmethod translate-from-foreign (x (type buffer-type)) (make-instance 'buffer :buffer-ptr x))
(defcfun (buffer-create "fae_buffer_create") buffer (a :int32))
(defcfun (buffer-wrap "fae_buffer_wrap") buffer (a ptr) (b :int32) (c unary) (d ptr))
(defcfun (buffer-copy "fae_buffer_copy") buffer (a buffer))
(defcfun (buffer-resize "fae_buffer_resize") buffer (a :int32) (b buffer))
(defcfun (buffer-destroy "fae_buffer_destroy") :void (a buffer))
(defcfun (buffer-size "fae_buffer_size") :int32 (a buffer))
(defcfun (buffer-get "fae_buffer_get") :uint8 (a buffer) (b :int32))
(defcfun (buffer-set "fae_buffer_set") :void (a buffer) (b :int32) (c :uint8))
(defcfun (buffer-get-int16 "fae_buffer_get_int16") :int16 (a buffer) (b :int32))
(defcfun (buffer-get-int32 "fae_buffer_get_int32") :int32 (a buffer) (b :int32))
(defcfun (buffer-get-int64 "fae_buffer_get_int64") :int64 (a buffer) (b :int32))
(defcfun (buffer-get-float "fae_buffer_get_float") :float (a buffer) (b :int32))
(defcfun (buffer-get-double "fae_buffer_get_double") :double (a buffer) (b :int32))
(defcfun (buffer-set-int16 "fae_buffer_set_int16") :void (a buffer) (b :int32) (c :int16))
(defcfun (buffer-set-int32 "fae_buffer_set_int32") :void (a buffer) (b :int32) (c :int32))
(defcfun (buffer-set-int64 "fae_buffer_set_int64") :void (a buffer) (b :int32) (c :int64))
(defcfun (buffer-set-float "fae_buffer_set_float") :void (a buffer) (b :int32) (c :float))
(defcfun (buffer-set-double "fae_buffer_set_double") :void (a buffer) (b :int32) (c :double))
(defcfun (buffer-read-raw "fae_buffer_read_raw") buffer (a string-file-path))
(defcfun (buffer-write-raw "fae_buffer_write_raw") :void (a string-file-path) (b buffer))
(defcfun (buffer-read-audio "fae_buffer_read_audio") pair (a string-file-path))
(defcfun (buffer-write-audio "fae_buffer_write_audio") :void (a string-file-path) (b type) (c buffer))
(defcfun (buffer-unsafe-address "fae_buffer_unsafe_address") (:pointer :void) (a buffer))