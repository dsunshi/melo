#define ${defaults['entry_action']}  ((${defaults['action_type']}) 0u)
#define ${defaults['during_action']} ((${defaults['action_type']}) 1u)
#define ${defaults['exit_action']}   ((${defaults['action_type']}) 2u)
#define _AFTER(x) x

typedef ${defaults['state_id_type']} (*_state_func)(const ${defaults['action_type']} action, const ${defaults['event_type']} event);

typedef struct
{
	_state_func  function;
    ${defaults['rl_type']} left;
    ${defaults['rl_type']} right;
    ${defaults['timer_type']} timer;
} _state_handle;