#lang scheme
; 2016400231

;Finds data of an element in the list if the element exist
(define (findData Element List) (if (equal? (length List) 0)
                                     null
                                     (if (equal? (caar List) Element)
                                         (car List)
                                         (findData Element (cdr List))
                                         )
                                     ))

;Checks is an element in the list or not
(define (findElement Element List) (if (equal? (length List) 0)
                                     #f
                                     (if (equal? (car List) Element)
                                         #t
                                         (findElement Element (cdr List))
                                         )
                                     ))

;TRANSPORTATION-COST
(define (TRANSPORTATION-COST farm) (let ((currentFarm (findData farm FARMS)))
                                     (if (equal? currentFarm null)
                                         0
                                         (cadr currentFarm)
                                         )))
;AVAILABLE-CROPS
(define (AVAILABLE-CROPS farm) (let ((farmData (findData farm FARMS)))
                                     (if (equal? farmData null)
                                         '()
                                         (caddr farmData)
                                         )))
;INTERESTED-CROPS
(define (INTERESTED-CROPS customer) (let ((customerData (findData customer CUSTOMERS)))
                                      (if (equal? customerData null)
                                          '()
                                          (caddr customerData)
                                          )))
;CONTRACT-FARMS
(define (CONTRACT-FARMS customer) (let ((customerData (findData customer CUSTOMERS)))
                                      (if (equal? customerData null)
                                          '()
                                          (cadr customerData)
                                          )))
;returns if the customer has a contract with a particular farm
(define (contractHelper farm customer) (findElement farm (cadr customer)))

;takes the key elements of the list
(define (takeKeys List) (map (lambda(y)(car y)) List))

;CONTRACT-WITH-FARM
(define (CONTRACT-WITH-FARM farm) (takeKeys (filter (lambda(x) (contractHelper farm x)) CUSTOMERS)))

;returns if the customer is interested or not to a particular crop
(define (interestHelper crop customer) (findElement crop (caddr customer)))

;INTERESTED-IN-CROP
(define (INTERESTED-IN-CROP crop) (takeKeys (filter (lambda(x) (interestHelper crop x)) CUSTOMERS)))

(define (MSPhelper List lowest) (if (equal? (length List) 0)
                                     (if (equal? lowest +inf.0)
                                         0
                                         lowest
                                         )
                                     (if (<= lowest (caddar List))
                                         (MSPhelper (cdr List) lowest)
                                         (MSPhelper (cdr List) (caddar List))
                                         )
                                     ))

;MIN-SALE-PRICE
(define (MIN-SALE-PRICE crop) (MSPhelper (filter (lambda(x) (equal? crop (car x))) CROPS) +inf.0 ))

;CROPS-BETWEEN
(define (CROPS-BETWEEN min max) (remove-duplicates (takeKeys (filter (lambda(x) (and (<= (caddr x) max) (>= (caddr x) min))) CROPS))))

;BPhelper
(define (BPhelper farm crop) (let*(
                                   (availableCrops (filter (lambda(y) (and (equal? (car y) crop) (equal? (cadr y) farm))) CROPS))
                                   (farmData (findData farm FARMS)))
                               (if (null? farmData)
                                   +inf.0
                                   (if (null? availableCrops)
                                       +inf.0
                                       (+ (cadr farmData) (caddar availableCrops)))
                                   )))

;Min of the list
(define (minElement List lowest) (if (equal? (length List) 0)
                                     (if (equal? lowest +inf.0)
                                         0
                                         lowest
                                         )
                                     (if (<= lowest (car List))
                                         (minElement (cdr List) lowest)
                                         (minElement (cdr List) (car List))
                                         )
                                     ))

;BUY-PRICE
(define (BUY-PRICE customer crop) (if (null? (findData customer CUSTOMERS))
                                      0
                                      (minElement (map (lambda(x) (BPhelper x crop)) (cadr (findData customer CUSTOMERS))) +inf.0)
                                      ))

;TOTAL-PRICE
(define (TOTAL-PRICE customer) (let ((customerData (findData customer CUSTOMERS)))
                                 (if (null? customerData)
                                     0
                                     (foldr + 0 (map (lambda(x) (BUY-PRICE customer x)) (caddr customerData))))))