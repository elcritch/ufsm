TESTS  = test_simple
TESTS += test_simple_substate
TESTS += test_guards_actions
TESTS += test_guard_repeat
TESTS += test_data_model
TESTS += test_compound_transition
TESTS += test_xmi_machine
TESTS += test_deephistory
TESTS += test_fork
TESTS += test_join
TESTS += test_stack
TESTS += test_choice
TESTS += test_queue
TESTS += test_junction
TESTS += test_defer
TESTS += test_terminate
TESTS += test_conref
TESTS += test_transition_prio
TESTS += test_do
#TESTS += test_argouml
TESTS += test_choice2

CC ?= gcc
UFSMIMPORT ?= ufsmimport

UFSM_TESTS_VERBOSE ?= false

LDFLAGS ?= -lm

ifdef COVERAGE
LDFLAGS += -lgcov
endif


CFLAGS  = -g -O2 -Wall -Wextra -std=c99 -fprofile-arcs -ftest-coverage -Wno-unused-parameter

CFLAGS += -I.. -I. -I gen/ -I../tinyexpr/ -DUFSM_TESTS_VERBOSE=$(UFSM_TESTS_VERBOSE) -DUFSM_DEBUG -DTE_VALUE_TYPE=int

C_SRCS = ../ufsm.c ../ufsm_stack.c ../ufsm_queue.c common.c
OBJS = $(C_SRCS:.c=.o)

all: $(TESTS)
	@$(foreach TEST,$(TESTS), \
		echo && \
		echo "TEST $(TEST)" && \
		./$(TEST))
	@echo "*** ALL $(words ${TESTS}) TESTS PASSED ***"
%.o : %.c
	@echo CC $<
	@$(CC) -c $(CFLAGS) $< -o $@

%.c : %.xmi
	@echo UFSMIMPORT $< 
	@mkdir -p gen
	@$(UFSMIMPORT) $< $(patsubst %.xmi, %, $(<)) -c gen/

clean:
	@$(foreach TEST,$(TESTS), rm -f $(TEST);)
	@rm -rf gen/
	@rm -f *.o
	@rm -f *.gcda 
	@rm -f *.gcno
	@rm -f *.gcov

# TEST DEFINITIONS ###########################################################

test_xmi_machine: $(OBJS) test_xmi_machine_input.c test_xmi_machine.o
	@echo LINK $@
	@$(CC) $@.c gen/test_xmi_machine_input.c $(OBJS) $(CFLAGS) $(LDFLAGS) -o $@

test_fork: $(OBJS) test_fork_input.c test_fork.o
	@echo LINK $@
	@$(CC) $@.c gen/test_fork_input.c $(OBJS) $(CFLAGS) $(LDFLAGS) -o $@

test_join: $(OBJS) test_join_input.c test_join.o
	@echo LINK $@
	@$(CC) $@.c gen/test_join_input.c $(OBJS) $(CFLAGS) $(LDFLAGS) -o $@

test_compound_transition: $(OBJS) test_compound_transition_input.c test_compound_transition.o
	@echo LINK $@
	@$(CC) $@.c gen/test_compound_transition_input.c $(OBJS) $(CFLAGS) $(LDFLAGS) -o $@

test_deephistory: $(OBJS) test_deephistory_input.c test_deephistory.o
	@echo LINK $@
	@$(CC) $@.c gen/test_deephistory_input.c $(OBJS) $(CFLAGS) $(LDFLAGS) -o $@

test_simple: $(OBJS) test_simple.o
	@echo LINK $@
	@$(CC) $@.c $(OBJS) $(CFLAGS) $(LDFLAGS) -o $@

test_simple_substate: $(OBJS) test_simple_substate.o
	@echo LINK $@
	@$(CC) $@.c $(OBJS) $(CFLAGS) $(LDFLAGS) -o $@

test_data_model: $(OBJS) test_data_model.o  ../tinyexpr/tinyexpr.o
	@echo LINK $@
	$(CC) $@.c ../tinyexpr/tinyexpr.o $(OBJS) $(CFLAGS) $(LDFLAGS) -o $@

test_guards_actions: $(OBJS) test_guards_actions.o
	@echo LINK $@
	$(CC) $@.c $(OBJS) $(CFLAGS) $(LDFLAGS) -o $@

test_guard_repeat: $(OBJS) test_guard_repeat.o
	@echo LINK $@
	@$(CC) $@.c $(OBJS) $(CFLAGS) $(LDFLAGS) -o $@

test_stack: $(OBJS) test_stack.o
	@echo LINK $@
	@$(CC) $@.c $(OBJS) $(CFLAGS) $(LDFLAGS) -o $@

test_choice: $(OBJS) test_choice_input.c test_choice.o
	@echo LINK $@
	@$(CC) $@.c gen/test_choice_input.c $(OBJS) $(CFLAGS) $(LDFLAGS) -o $@

test_queue: $(OBJS) test_terminate_input.c test_queue.o
	@echo LINK $@
	@$(CC) $@.c gen/test_terminate_input.c $(OBJS) $(CFLAGS) $(LDFLAGS) -o $@

test_junction: $(OBJS) test_junction_input.c test_junction.o
	@echo LINK $@
	@$(CC) $@.c gen/test_junction_input.c $(OBJS) $(CFLAGS) $(LDFLAGS) -o $@

test_defer: $(OBJS) test_defer_input.c test_defer.o
	@echo LINK $@
	@$(CC) $@.c gen/test_defer_input.c $(OBJS) $(CFLAGS) $(LDFLAGS) -o $@

test_terminate: $(OBJS) test_terminate_input.c test_terminate.o
	@echo LINK $@
	@$(CC) $@.c gen/test_terminate_input.c $(OBJS) $(CFLAGS) $(LDFLAGS) -o $@

test_conref: $(OBJS) test_conref_input.c test_conref.o
	@echo LINK $@
	@$(CC) $@.c gen/test_conref_input.c $(OBJS) $(CFLAGS) $(LDFLAGS) -o $@

test_transition_prio: $(OBJS) test_transition_prio_input.c test_transition_prio.o
	@echo LINK $@
	@$(CC) $@.c gen/test_transition_prio_input.c $(OBJS) $(CFLAGS) $(LDFLAGS) -o $@

test_do: $(OBJS) test_do_input.c test_do.o
	@echo LINK $@
	@$(CC) $@.c gen/test_do_input.c $(OBJS) $(CFLAGS) $(LDFLAGS) -o $@

test_argouml: $(OBJS) test_argouml_input.c test_argouml.o
	@echo LINK $@
	@$(CC) $@.c gen/test_argouml_input.c $(OBJS) $(CFLAGS) $(LDFLAGS) -o $@

test_choice2: $(OBJS) test_choice2_input.c test_choice2.o
	@echo LINK $@
	@$(CC) $@.c gen/test_choice2_input.c $(OBJS) $(CFLAGS) $(LDFLAGS) -o $@

