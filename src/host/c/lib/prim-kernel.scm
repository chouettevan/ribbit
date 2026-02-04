(define-primitive (get-interrupt)
  "{
  if (irqs.i == 0) {
    push2(FALSE,PAIR_TAG);
  } else {
    int __number = irqs.data[irqs.i-1];
    irqs.i--;
    push2(TAG_NUM(__number), PAIR_TAG);
  }
  break;
  }")

(define-primitive (outb port number)
  "{
  PRIM2();
  outb((unsigned short)NUM(x),(unsigned char)NUM(y));
  push2(FALSE,PAIR_TAG);
  break;
  }")

(define-primitive (inb port)
  "{
  PRIM1();
  push2(TAG_NUM((int)inb((unsigned short)NUM(x))),PAIR_TAG); 
  break;
  }")

(define-primitive (lor a b)
  "{
  PRIM2();
  push2(TAG_NUM(NUM(x) | NUM(y)),PAIR_TAG); 
  break;
  }")

(define-primitive (land a b)
  "{
  PRIM2();
  push2(TAG_NUM(NUM(x) & NUM(y)),PAIR_TAG); 
  break;
  }")

(define-primitive (lxor a b)
  "{
  PRIM2();
  push2(TAG_NUM(NUM(x) ^ NUM(y)),PAIR_TAG); 
  break;
  }")

(define-primitive (lshift a b)
  "{
  PRIM2();
  push2(TAG_NUM(NUM(x) << NUM(y)),PAIR_TAG); 
  break;
  }")

