${defaults['bool_type']} _is_parent(const _state_handle * const child, const _state_handle * const parent)
{
    ${defaults['bool_type']} result = 0;
    
    if ( (parent->left < child->left) && (parent->right > child->right) )
    {
        result = ${defaults['true']};
    }
    else
    {
        result = ${defaults['false']};
    }
    
    return result;
}

${defaults['state_id_type']} _state_transition(${defaults['state_id_type']} start_state, ${defaults['state_id_type']} dest_state)
{
    ${defaults['state_id_type']} index;
    
    /* Exit start_state */
    (void) ${defaults['table_name']}[start_state].function(${defaults['exit_action']}, 0);
    
    for (index = start_state; index > 0; index--)
    {
        if (_is_parent( &(${defaults['table_name']}[start_state]), &(${defaults['table_name']}[index]) ) != ${defaults['false']})
        {
            if (_is_parent( &(${defaults['table_name']}[dest_state]), &(${defaults['table_name']}[index]) ) == ${defaults['false']})
            {
                (void) ${defaults['table_name']}[index].function(${defaults['exit_action']}, 0);
            }
            else
            {
                /* Do nothing - we are not actually exiting the parent state - only "touching" it */
            }
        }
        else
        {
            /* Do nothing - not a parent state */
        }
    }
    
    for (index++; index <= dest_state; index++)
    {
        if (_is_parent( &(${defaults['table_name']}[dest_state]), &(${defaults['table_name']}[index]) ) != ${defaults['false']})
        {
            if (_is_parent( &(${defaults['table_name']}[start_state]), &(${defaults['table_name']}[index]) ) == ${defaults['false']})
            {
                (void) ${defaults['table_name']}[index].function(${defaults['entry_action']}, 0);
            }
            else
            {
                /* Do nothing - we are not actually exiting the parent state - only "touching" it */
            }
        }
        else
        {
            /* Do nothing - not a parent state */
        }
    }
    
    /* Enter dest_state */
    (void) ${defaults['table_name']}[dest_state].function(${defaults['entry_action']}, 0);
    
    return dest_state;
}