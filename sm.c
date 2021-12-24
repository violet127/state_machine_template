#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_FREE_DRINK 3
#define MAX_PEOPLE 5
int free_drink_left;

typedef enum st {
   ST_START = 0,
   ST_TAKE_FREE_DRINK,
   ST_BUY_DRINK,
   ST_END,
} st_t;

typedef enum st_action { FORWARD = 0, BACK = 1, STOP = 2 } st_action_t;

struct sm_data {
   st_t next_state;
   int id;
   int action;
   int stop;
};

typedef struct st_fn {
   st_t (*sfn)();
} state_fn_t;

static st_t state_start();
static st_t state_take_free_drink();
static st_t state_buy_drink();
static st_t state_end();

static state_fn_t state_func[] = {
    [ST_START] = {state_start},
    [ST_TAKE_FREE_DRINK] = {state_take_free_drink},
    [ST_BUY_DRINK] = {state_buy_drink},
    [ST_END] = {state_end}};

st_t state_start(struct sm_data *sm) {
   printf("man[%d]: start\n", sm->id);
   if (free_drink_left > 0) {
      sm->next_state = ST_TAKE_FREE_DRINK;
   } else {
      sm->next_state = ST_BUY_DRINK;
   }
}

st_t state_take_free_drink(struct sm_data *sm) {
   printf("man[%d]: take free drink\n", sm->id);
   free_drink_left--;
   sm->next_state = ST_END;
}

st_t state_buy_drink(struct sm_data *sm) {
   printf("man[%d]: buying free drink...\n", sm->id);
   sleep(1);
   sm->next_state = ST_END;
}

st_t state_end(struct sm_data *sm) {
   printf("man[%d]: end\n", sm->id);
   sm->stop = 1;
}

st_t state_action(struct sm_data *sm) {
   if (1 != sm->stop) {
      return (*state_func[sm->next_state].sfn)((void *)sm);
   }
}

void run_st(struct sm_data *sm) {
   while (1 != sm->stop) {
      st_t ret = state_action(sm);
      sleep(1);
   }
}

int main(void) {
   free_drink_left = MAX_FREE_DRINK;
   printf("--- state machine start ---\n");
   for (int i = 0; i < MAX_PEOPLE; i++) {
      struct sm_data sm;
      memset(&sm, 0, sizeof(sm));
      sm.id = i + 1;
      sm.next_state = ST_START;
      run_st(&sm);
   }
   printf("--- state machine stop ---\n");
   return 0;
}
