<%namespace name="airy" file="airy.tpl"/>

${defaults['static']} _state_handle ${defaults['table_name']}[${len(states)}] =
{
    /* State Name, Left, Right, Timer */
% for state in states:
    /* ${state['id']} */ {${airy.build_func_name(state['name'])}, ${state['left']}, ${state['right']}, 0},
% endfor
};

${defaults['static']} ${defaults['state_id_type']} _current_state = 0;