<%!
import re

after_pattern = re.compile('AFTER\(\d+\)')

def isafter_gaurd(text):
    return after_pattern.match(text)

%>

<%namespace name="airy" file="airy.tpl"/>

% for state in states:
/* State ${state['name']} */
${defaults['static']} ${defaults['state_id_type']} ${airy.build_func_name(state['name'])}(const ${defaults['action_type']} action, const ${defaults['event_type']} event)
{
    ${defaults['state_id_type']} result = ${state['id']};
    
    if (action == ${defaults['entry_action']})
    {
% if state['timer'] != False:
        ${defaults['table_name']}[${state['id']}].timer = 0;
% endif
% if len(state['entry']) > 0:
        ${state['entry']}
% endif
    }
    else if (action == ${defaults['during_action']})
    {
% if state['timer'] != False:
        ${defaults['table_name']}[${state['id']}].timer++;
% endif
% if state['parent'] != state['id']:
        (void) ${defaults['table_name']}[${state['parent']}].function(${defaults['during_action']}, event);
% endif
% if len(state['during']) > 0:
        ${state['during']}
% endif
% for transition in state['transitions']:
% if isafter_gaurd(transition['gaurd']):
        if (${defaults['table_name']}[${state['id']}].timer > _${transition['gaurd']})
% else:
        if (${transition['gaurd']})
% endif
        {
% if len(transition['action']) > 0:
            ${transition['action']}
% endif
% if transition['dest'] != state['id']:
            result = _state_transition(${state['id']}, ${transition['dest']});
% endif
        }
% endfor
    }
    else if (action == ${defaults['exit_action']})
    {
% if len(state['exit']) > 0:
        ${state['exit']}
% endif
    }
    else
    {
        /* Error - ??? */
    }
    
    return result;
}
% endfor