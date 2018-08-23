# create bash and make files, compile the code

import os
import subprocess as sb
import numpy as np
import itertools
from partition import partition_repr

run_on_cluster = True
node = 'broadwell'

runtime = '16:00:00'

# === bash parameters ====
# number of cores
N_core = 1

# name of executable: 'simulation' or 'test_simulation'
exec_name = 'simulation-contd'

if run_on_cluster:
    compiler = 'CPP = g++'
else:
    compiler = 'CPP = g++' # 'CPP = g++-4.9'

cpp_files = ['clock', 'csv_reader', 'node', 'params', 'pop', exec_name]

# === make and compile ===

# clean old temp files and remove old makefile
if os.path.isfile('makefile'):
    sb.call('make clean', shell=True, cwd = os.getcwd())
    os.remove('makefile')

# create makefile
sb.call('touch makefile', shell=True, cwd = os.getcwd())

# makefile content
line = '{0}\n\nFLAGS = -std=c++11 -O3'.format(compiler)

line_comp = ''
line_link = ''
line_all  = ''
for f in cpp_files:
    line_comp += '\n\n{0} :\n\t$(CPP) $(FLAGS) -c {0}.cpp'.format(f)
    line_link += ' {}.o'.format(f)
    line_all  += ' {}'.format(f)

line += line_comp
line += """

link :
\t$(CPP) $(FLAGS) -o {0}{1}

all :{2} link

clean :
\trm -f {0}{1}""".format(exec_name, line_link, line_all)

# write makefile
with open('makefile', 'w') as f:
    f.write(line)

# purge and load modules
if run_on_cluster:
    sb.call('module purge', shell=True, cwd=os.getcwd())
    # next 2 lines are for loading boost library
    sb.call('module use /projects/community/modulefiles', shell=True, cwd=os.getcwd())
    sb.call('module load  boost/1.66.0-gc563', shell=True, cwd=os.getcwd())
    sb.call('module load python', shell=True, cwd=os.getcwd())
    sb.call('module load gcc', shell=True, cwd=os.getcwd())
# compile source file
sb.call('make all', shell=True, cwd = os.getcwd())
# path to the executable
exec_path = os.path.join(os.getcwd(), exec_name)

for dirpath, dirnames, filenames in os.walk(os.getcwd()):
    if ('pop.txt' in filenames):
        # copy the executable to the subfolder
        sb.call('cp {0} {1}'.format(exec_path, dirpath), shell=True)

        # === bash ====
        if run_on_cluster:
            bash_name = 'run.sh'
            sb.call('touch {}'.format(bash_name), shell=True, cwd=dirpath)

            # bash file content
            line = """#!/bin/bash

#SBATCH --constraint={node}
#SBATCH --partition=main
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=1
#SBATCH --mem=2000
#SBATCH --time={runtime}
#SBATCH --output=slurm.%N.%j.out
#SBATCH --export=ALL
./{exec_name}""".format(runtime=runtime, exec_name=exec_name, node=node)

            # write bash file
            with open(os.path.join(dirpath, bash_name), 'w') as f:
                f.write(line)
        
        # === run ===
        if run_on_cluster:
            sb.call('sbatch {}'.format(bash_name), shell=True, cwd=dirpath)
        else:
            sb.call('./{}'.format(exec_name), shell=True, cwd=dirpath)

# clean up
if os.path.isfile('makefile'):
    sb.call('make clean', shell=True, cwd = os.getcwd())
    os.remove('makefile')