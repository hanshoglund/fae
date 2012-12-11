(defctype Doremir.String (:pointer :char))

(defctype Doremir.String.FilePath :Doremir.String)

(defcfun "Doremir.String.create" :Doremir.String ())

(defcfun "Doremir.String.length" :int (:Doremir.String))

(defcfun "Doremir.String.length" :int (:Doremir.String))

(defctype Doremir.String.Unary (:pointer (:pointer :void)))

(defctype Doremir.String.Binary (:pointer (:pointer :void)))

(defcfun "Doremir.String.map" :Doremir.String (:Doremir.String :Doremir.String.Unary))

(defcfun "Doremir.String.fold" :Doremir.String (:Doremir.String :Doremir.String.Binary :char))