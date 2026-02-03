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
  outb((unsigned short)UNTAG(x),(unsigned char)UNTAG(y));
  break;
  }")

(define-primitive (inb port)
  "{
  PRIM1();
  push2(TAG_NUM(inb((unsigned short)UNTAG(x))),PAIR_TAG); 
  break;
  }")
