/*  debug.c -- optional debugging utilities */
/*  Copyright (c) 2009 Alex Shinn.  All rights reserved. */
/*  BSD-style license: http://synthcode.com/license.txt */

static const char* reverse_opcode_names[] =
  {"NOOP", "STACK_REF", "STACK_SET", "GLOBAL_REF", "GLOBAL_SET", "CLOSURE_REF",
   "CLOSURE_SET", "VECTOR_REF", "VECTOR_SET", "MAKE_PROCEDURE", "MAKE_VECTOR",
   "PUSH", "DUP", "DROP", "SWAP", "CAR", "CDR", "SET_CAR", "SET_CDR", "CONS",
   "ADD", "SUB", "MUL", "DIV", "MOD", "NEG", "INV", "LT", "CALL",
   "JUMP_UNLESS", "JUMP", "RET", "DONE"
  };

void disasm (bytecode bc) {
  unsigned char *ip=bc->data, opcode;
 loop:
  opcode = *ip++;
  if (opcode*sizeof(char*) < sizeof(reverse_opcode_names)) {
    fprintf(stderr, "  %s ", reverse_opcode_names[opcode]);
  } else {
    fprintf(stderr, "  <unknown> %d ", opcode);
  }
  switch (opcode) {
  case OP_STACK_REF:
  case OP_STACK_SET:
  case OP_CLOSURE_REF:
  case OP_CLOSURE_SET:
    fprintf(stderr, "%d", (long) ((sexp*)ip)[0]);
    ip += sizeof(sexp);
    break;
  case OP_GLOBAL_REF:
  case OP_GLOBAL_SET:
  case OP_CALL:
  case OP_PUSH:
    write_sexp(stderr, ((sexp*)ip)[0]);
    ip += sizeof(sexp);
    break;
  case OP_JUMP:
  case OP_JUMP_UNLESS:
    fprintf(stderr, "%d", ip[0]);
    ip++;
    break;
  }
  fprintf(stderr, "\n");
  if ((! (opcode == OP_RET) || (opcode == OP_DONE))
      && (ip - bc->data < bc->len))
    goto loop;
}

void print_bytecode (bytecode bc) {
  int i;
  fprintf(stderr, "bytecode @ %p, data @ %p, length = %d\n",
          bc, bc->data, bc->len);
  for (i=0; i+16 < bc->len; i+=8) {
    fprintf(stderr, "%02x: %02x %02x %02x %02x %02x %02x %02x %02x   ", i,
            bc->data[i], bc->data[i+1], bc->data[i+2], bc->data[i+3],
            bc->data[i+4], bc->data[i+5], bc->data[i+6], bc->data[i+7]);
    i += 8;
    fprintf(stderr, "%02x %02x %02x %02x %02x %02x %02x %02x\n",
            bc->data[i], bc->data[i+1], bc->data[i+2], bc->data[i+3],
            bc->data[i+4], bc->data[i+5], bc->data[i+6], bc->data[i+7]);
  }
  if (i != bc->len) {
    fprintf(stderr, "%02x:", i);
    for ( ; i < bc->len; i++) {
      if ((i % 8) == 0 && (i % 16) != 0)
        fprintf(stderr, "  ");
      fprintf(stderr, " %02x", bc->data[i]);
    }
    fprintf(stderr, "\n");
  }
}

void print_stack (sexp *stack, int top) {
  int i;
  for (i=0; i<top; i++) {
    fprintf(stderr, "  %02d: ", i);
    fflush(stderr);
    write_sexp(stderr, stack[i]);
    fprintf(stderr, "\n");
  }
}
