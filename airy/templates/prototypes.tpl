<%namespace name="airy" file="airy.tpl"/>

/* States */
% for state in states:
${defaults['static']} ${defaults['state_id_type']} ${airy.build_func_name(state['name'])}(const ${defaults['action_type']} action, const ${defaults['event_type']} event);
% endfor

/* Builtin Functions */
${defaults['bool_type']} _is_parent(const _state_handle * const child, const _state_handle * const parent);
${defaults['state_id_type']} _state_transition(${defaults['state_id_type']} start_state, ${defaults['state_id_type']} dest_state);