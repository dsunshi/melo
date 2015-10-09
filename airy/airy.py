#
# Copyright (c) 2015 David Sunshine, <http://sunshin.es>
# 
# This file is part of melo.
# 
# melo is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 2 of the License, or
# (at your option) any later version.
# 
# melo is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with melo.  If not, see <http://www.gnu.org/licenses/>.
#

import yaml
import sys
import getopt
import inspect
import os
from mako.template import Template
from mako.lookup import TemplateLookup

defaults = dict(
    table_name    = '_table',
    event_type    = 'uint8_t',
    action_type   = 'uint8_t',
    state_id_type = 'uint16_t',
    timer_type    = 'uint16_t',
    rl_type       = 'uint8_t',
    bool_type     = 'bool',
    true          = 'true',
    false         = 'false',
    entry_action  = '_STATE_ACTION_ENTRY',
    during_action = '_STATE_ACTION_DURING',
    exit_action   = '_STATE_ACTION_EXIT',
    state_prefix  = '_',
    state_suffix  = '_',
    static        = 'static'
)

states = [
    dict(
        name   = 'IDLE',
        id     = 0,
        left   = 1,
        right  = 2,
        entry  = '',
        during = '',
        exit   = '',
        timer  = False,
        parent = 0,
        transitions = [
            dict(
                gaurd  = 'event == MELO_EVENT_REQUEST_RECEIVED',
                action = '',
                dest   = 2
            )
        ]
    ),
    dict(
        name   = 'RESP_PROC',
        left   = 3,
        right  = 8,
        id     = 1,
        entry  = '_melo_frame_handler(&(recv_frame.frame), false);',
        during = '',
        exit   = '',
        timer  = True,
        parent = 1,
        transitions = [
            dict(
                gaurd  = 'AFTER(500)',
                action = '',
                dest   = 0
            )
        ]
    ),
    dict(
        name   = 'RESP_PEND',
        left   = 4,
        right  = 5,
        id     = 2,
        entry  = 'MeloTransmitBytes( &(wait_frame.buffer.data[0]), wait_frame.buffer.length );',
        during = '',
        exit   = '',
        timer  = False,
        parent = 1,
        transitions = [
            dict(
                gaurd  = 'event == MELO_EVENT_REQUEST_RECEIVED',
                action = '',
                dest   = 2
            ),
            dict(
                gaurd  = 'event == MELO_EVNET_TX_CONFIRMATION',
                action = '',
                dest   = 3
            )
        ]
    ),
    dict(
        name   = 'TX_PEND',
        left   = 6,
        right  = 7,
        id     = 3,
        parent = 1,
        entry  = 'MeloTransmitBytes( &(send_frame.buffer.data[0]), send_frame.buffer.length );',
        during = '',
        exit   = '',
        timer  = False,
        transitions = [
            dict(
                gaurd  = 'event == MELO_EVENT_REQUEST_RECEIVED',
                action = '',
                dest   = 3
            ),
            dict(
                gaurd  = 'event == MELO_EVNET_TX_CONFIRMATION',
                action = '',
                dest   = 0
            )
        ]
    )
]

#with open('airy.yml', 'w') as outfile:
#    outfile.write(yaml.dump(states, default_flow_style=True))



#for key, value in yaml.load(open('airy.yml')).iteritems():
#    print key, value

def main(argv):
    inputfile  = ''
    outputfile = ''
    try:
        opts, args = getopt.getopt(argv,"hi:o:",["ifile=","ofile="])
    except getopt.GetoptError:
        print 'airy.py -i <inputfile> -o <outputfile>'
        sys.exit(2)
    for opt, arg in opts:
        if opt == '-h':
            print 'airy.py -i <inputfile> -o <outputfile>'
            sys.exit()
        elif opt in ("-i", "--ifile"):
            inputfile = arg
        elif opt in ("-o", "--ofile"):
            outputfile = arg
    print 'Input file is "', inputfile
    print 'Output file is "', outputfile
    
    lookup = TemplateLookup(directories=[ os.path.dirname(os.path.abspath(inspect.getfile(inspect.currentframe()))) ])
    template_file = Template(filename=inputfile, lookup=lookup)
    f = open(outputfile, "w")
    f.write(template_file.render(states=states, defaults=defaults))
    f.close()

if __name__ == "__main__":
    main( sys.argv[1:] )
