(in-package :faudio)
(define-foreign-type signal-type () () (:actual-type :pointer))
(define-parse-method signal () (make-instance 'signal-type))
(defclass signal () ((signal-ptr :initarg :signal-ptr)))
(defmethod translate-to-foreign (x (type signal-type)) (slot-value x 'signal-ptr))
(defmethod translate-from-foreign (x (type signal-type)) (make-instance 'signal :signal-ptr x))
(defctype signal-unary-signal (:pointer (:pointer :void)))
(defctype signal-unary-double (:pointer (:pointer :void)))
(defctype signal-binary-double (:pointer (:pointer :void)))
(defcfun (signal-time "fa_signal_time") signal)
(defcfun (signal-random "fa_signal_random") signal)
(defcfun (signal-constant "fa_signal_constant") signal (a :double))
(defcfun (signal-lift "fa_signal_lift") signal (a string) (b :pointer) (c :pointer) (d signal))
(defcfun (signal-lift2 "fa_signal_lift2") signal (a string) (b :pointer) (c :pointer) (d signal) (e signal))
(defcfun (signal-loop "fa_signal_loop") signal (a :pointer) (b :pointer))
(defcfun (signal-delay "fa_signal_delay") signal (a :int) (b signal))
(defcfun (signal-insert "fa_signal_insert") signal (a string) (b :int) (c :int) (d :int) (e :int) (f signal))
(defctype signal-name string)
(defctype signal-message ptr)
(defctype signal-state (:pointer :void))
(defctype signal-custom-processor (:pointer :void))
(defcfun (signal-add-custom-processor "fa_signal_add_custom_processor") signal (a (:pointer signal-custom-processor)) (b signal))
(defcfun (signal-input "fa_signal_input") signal (a :int))
(defcfun (signal-output "fa_signal_output") signal (a :int) (b :int) (c signal))
(defcfun (signal-latter "fa_signal_latter") signal (a signal) (b signal))
(defcfun (signal-former "fa_signal_former") signal (a signal) (b signal))
(defcfun (signal-print "fa_signal_print") :void (a :int) (b list) (c signal))
(defcfun (signal-run "fa_signal_run") :void (a :int) (b list) (c signal) (d (:pointer :double)))
(defcfun (signal-run-buffer "fa_signal_run_buffer") buffer (a :int) (b list) (c signal))
(defcfun (signal-run-file "fa_signal_run_file") ptr (a :int) (b list) (c signal) (d string))
(defcfun (signal-play "fa_signal_play") signal (a buffer) (b signal))
(defcfun (signal-record "fa_signal_record") signal (a buffer) (b signal) (c signal))
(defcfun (signal-add "fa_signal_add") signal (a signal) (b signal))
(defcfun (signal-subtract "fa_signal_subtract") signal (a signal) (b signal))
(defcfun (signal-multiply "fa_signal_multiply") signal (a signal) (b signal))
(defcfun (signal-power "fa_signal_power") signal (a signal) (b signal))
(defcfun (signal-divide "fa_signal_divide") signal (a signal) (b signal))
(defcfun (signal-modulo "fa_signal_modulo") signal (a signal) (b signal))
(defcfun (signal-absolute "fa_signal_absolute") signal (a signal))
(defcfun (signal-not "fa_signal_not") signal)
(defcfun (signal-and "fa_signal_and") signal (a signal) (b signal))
(defcfun (signal-or "fa_signal_or") signal (a signal) (b signal))
(defcfun (signal-xor "fa_signal_xor") signal (a signal) (b signal))
(defcfun (signal-bit-not "fa_signal_bit_not") signal (a signal) (b signal))
(defcfun (signal-bit-and "fa_signal_bit_and") signal (a signal) (b signal))
(defcfun (signal-bit-or "fa_signal_bit_or") signal (a signal) (b signal))
(defcfun (signal-bit-xor "fa_signal_bit_xor") signal (a signal) (b signal))
(defcfun (signal-shift-left "fa_signal_shift_left") signal (a signal) (b signal))
(defcfun (signal-shift-right "fa_signal_shift_right") signal (a signal) (b signal))
(defcfun (signal-equal "fa_signal_equal") signal (a signal) (b signal))
(defcfun (signal-less-than "fa_signal_less_than") signal (a signal) (b signal))
(defcfun (signal-greater-than "fa_signal_greater_than") signal (a signal) (b signal))
(defcfun (signal-less-than-equal "fa_signal_less_than_equal") signal (a signal) (b signal))
(defcfun (signal-greater-than-equal "fa_signal_greater_than_equal") signal (a signal) (b signal))
(defcfun (signal-acos "fa_signal_acos") signal (a signal))
(defcfun (signal-asin "fa_signal_asin") signal (a signal))
(defcfun (signal-atan "fa_signal_atan") signal (a signal))
(defcfun (signal-cos "fa_signal_cos") signal (a signal))
(defcfun (signal-sin "fa_signal_sin") signal (a signal))
(defcfun (signal-tan "fa_signal_tan") signal (a signal))
(defcfun (signal-exp "fa_signal_exp") signal (a signal))
(defcfun (signal-log "fa_signal_log") signal (a signal))
(defcfun (signal-log10 "fa_signal_log10") signal (a signal))
(defcfun (signal-pow "fa_signal_pow") signal (a signal) (b signal))
(defcfun (signal-sqrt "fa_signal_sqrt") signal (a signal))
(defcfun (signal-min "fa_signal_min") signal (a signal) (b signal))
(defcfun (signal-max "fa_signal_max") signal (a signal) (b signal))
(defcfun (signal-fmod "fa_signal_fmod") signal (a signal) (b signal))
(defcfun (signal-remainder "fa_signal_remainder") signal (a signal) (b signal))
(defcfun (signal-floor "fa_signal_floor") signal (a signal) (b signal))
(defcfun (signal-ceil "fa_signal_ceil") signal (a signal) (b signal))
(defcfun (signal-rint "fa_signal_rint") signal (a signal) (b signal))
(defcfun (signal-counter "fa_signal_counter") signal)
(defcfun (signal-impulses "fa_signal_impulses") signal (a :int))
(defcfun (signal-vst "fa_signal_vst") list (a string) (b string) (c list))
(defcfun (signal-fluid "fa_signal_fluid") list (a string))
(defcfun (signal-to-tree "fa_signal_to_tree") pair (a signal))
(defcfun (signal-draw-tree "fa_signal_draw_tree") string (a pair))
(defcfun (signal-simplify "fa_signal_simplify") signal (a signal))
(defcfun (signal-impulse "fa_signal_impulse") signal)
(defcfun (signal-line "fa_signal_line") signal (a :double))
(defcfun (signal-low-pass "fa_signal_low_pass") signal (a signal) (b signal) (c signal) (d signal) (e signal))
(defcfun (signal-biquad "fa_signal_biquad") signal (a signal) (b signal) (c signal) (d signal) (e signal) (f signal))

