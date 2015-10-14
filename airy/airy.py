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

def main(argv):
    inputfile  = ''
    outputfile = ''
    statefile  = ''
    try:
        opts, args = getopt.getopt(argv,"hi:o:s:",["ifile=","ofile=","sfile="])
    except getopt.GetoptError:
        print 'airy.py -i <inputfile> -o <outputfile> -s <statefile>'
        sys.exit(2)
    for opt, arg in opts:
        if opt == '-h':
            print 'airy.py -i <inputfile> -o <outputfile> -s <statefile>'
            sys.exit()
        elif opt in ("-i", "--ifile"):
            inputfile = arg
        elif opt in ("-o", "--ofile"):
            outputfile = arg
        elif opt in ("-s", "--sfile"):
            statefile = arg
    print 'Input file is "', inputfile
    print 'Output file is "', outputfile
    print 'Satefile file is "', statefile

    states = yaml.load( open(statefile) )
    
    lookup = TemplateLookup(directories=[ os.path.dirname(os.path.abspath(inspect.getfile(inspect.currentframe()))) ])
    template_file = Template(filename=inputfile, lookup=lookup)
    f = open(outputfile, "w")
    f.write(template_file.render(states=states, defaults=defaults))
    f.close()

if __name__ == "__main__":
    main( sys.argv[1:] )
