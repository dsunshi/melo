
<%def name="build_func_name(x)"><%return defaults['state_prefix'] + x + defaults['state_suffix']%></%def>

/******************************************************************************
*                              Local Data Types                               *
******************************************************************************/
<%include file="types.tpl" />

/******************************************************************************
*                          Local Function Prototypes                          *
******************************************************************************/
<%include file="prototypes.tpl" />

/******************************************************************************
*                               Local Variables                               *
******************************************************************************/
<%include file="variables.tpl" />

/******************************************************************************
*                          Local Function Definitions                         *
******************************************************************************/
<%include file="builtins.tpl" />

<%include file="states.tpl" />

int main(void)
{
    printf("Event: _REQUEST_RECEIVED\n");
    _current_state = ${defaults['table_name']}[_current_state].function(${defaults['during_action']}, _REQUEST_RECEIVED);
    printf("Event: _TX_CONFIRMATION\n");
    _current_state = ${defaults['table_name']}[_current_state].function(${defaults['during_action']}, _TX_CONFIRMATION);
    printf("Event: _REQUEST_RECEIVED\n");
    _current_state = ${defaults['table_name']}[_current_state].function(${defaults['during_action']}, _REQUEST_RECEIVED);
    printf("Event: _REQUEST_RECEIVED\n");
    _current_state = ${defaults['table_name']}[_current_state].function(${defaults['during_action']}, _REQUEST_RECEIVED);
    printf("Event: _REQUEST_RECEIVED\n");
    _current_state = ${defaults['table_name']}[_current_state].function(${defaults['during_action']}, _REQUEST_RECEIVED);
    printf("Event: _REQUEST_RECEIVED\n");
    _current_state = ${defaults['table_name']}[_current_state].function(${defaults['during_action']}, _REQUEST_RECEIVED);
    
    return 0;
}
