# create bash and make files, compile the code

import os
import subprocess as sb
import numpy as np
import itertools
from partition import partition_repr

run_on_cluster = True
node = 'broadwell'

def mu(theta, Npop):
    return theta / Npop

def rho(theta, Npop, rho_over_mu):
    return rho_over_mu * mu(theta, Npop)

def fitness_vec(nat_sel, Npop):
    Ns, Nds = nat_sel
    f = np.array([0.0, Ns - Nds, Ns + Nds])
    return 1.0 + f * 1.0 / Npop

Npop = 10 ** 3
Nitr = {
    0.1  : 8 * 10 ** 5,
    1.0  : 10 ** 6,
    10.0 : 10 ** 6,
}
Ngen = {
    0.1  : 512 * 10 ** 6,
    1.0  :  64 * 10 ** 6,
    10.0 :  16 * 10 ** 6,
}

# number of points where measure mean fitness
# when approaching steady state
Nepoch = 500

# fraction of population with the highest fitness
# when generating initial random population
top_frac = 0.0

# regimes after reaching steady state
scenario = 'const' # 'const', 'bottleneck', 'time_ss'
# bottleneck
Npop_lo = 267
Npop_hi = 1200
Ngen_lo = {
    0.1  : 128 * 10 ** 6,
    1.0  :  32 * 10 ** 6,
    10.0 :   8 * 10 ** 6,
}
Ngen_hi = {
    0.1  : 512 * 10 ** 6,
    1.0  :  64 * 10 ** 6,
    10.0 :  32 * 10 ** 6,
}

frac_3_top = 0.0322275162
frac_3_mid = 0.2099971771
frac_3_bot = 1 - frac_3_mid - frac_3_top
gamma_vec = [frac_3_bot, frac_3_mid, frac_3_top]

NetType = [
    'FC',
    'SPM',
]
SampleSize = [
    3,
    4,
    5,
]
Theta = [
    0.1,
    1.0,
    10.0,
]
NatSel = [
    (0.0, 0.0),
    (6.0, 0.0),
    (6.0, 3.0),
    (13.0, 0.0),
    (13.0, 5.0),
]
RhoOverMu = [
    0.0,
    1.0,
    2.0
]

lists = (NetType, SampleSize, Theta, NatSel, RhoOverMu)

runtime = {
    0.1 : '62:36:56',
    1.0 : '09:03:48',
    10.0: '01:31:14',
}
write_pop = {
    0.1  : 1, # write
    1.0  : 0,
    10.0 : 0,
}

# === bash parameters ====
# number of cores
N_core = 1

# name of executable: 'simulation' or 'test_simulation'
exec_name = 'simulation'

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

if os.path.isdir('data'):
    sb.call('rm -r data', shell=True, cwd = os.getcwd())
sb.call('mkdir data', shell=True, cwd=os.getcwd())
data_path = os.path.join(os.getcwd(), 'data')

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

for element in itertools.product(*lists):
    net_type, sample_size, theta, nat_sel, rho_over_mu = element
    Ns, Nds = nat_sel
    el = net_type, sample_size, theta, Ns, Nds, rho_over_mu
    
    # number of integer partitions for sampling prob (Npar)
    Npar = len(partition_repr(sample_size))
    
    if net_type == 'FC' and rho_over_mu:
        # no recombination for FC
        pass
    else:
        # === create and populate folders ===
        dir_name = '{0}_n-{1:d}_theta-{2:0.1f}_Ns-{3:0.0f}-{4:0.0f}_RhoMuRatio-{5:0.0f}'.format(*el)
        # create the folder
        sb.call('mkdir {}'.format(dir_name), shell=True, cwd=data_path)
        # path to the folder
        dir_path = os.path.join(data_path, dir_name)
        
        # copy the executable to the subfolder
        sb.call('cp {0} {1}'.format(exec_path, dir_path), shell=True)
        
        # === param ===
        # param file content
        line = str()
        line += 'net_type,{}\n'.format(net_type)
        line += 'Npop,{:d}\n'.format(Npop)
        if net_type == 'FC':
            line += 'Lalp,{:d}\n'.format(4 ** 10)
            line += 'Lseq,{:d}\n'.format(1)
        if net_type == 'SPM':
            line += 'Lalp,{:d}\n'.format(4)
            line += 'Lseq,{:d}\n'.format(10)
        line += 'mu,{:0.10f}\n'.format(mu(theta, Npop))
        line += 'rho,{:0.10f}\n'.format(rho(theta, Npop, rho_over_mu))
        line += 'fitness'
        for f in fitness_vec(nat_sel, Npop):
            line += ',{:0.10f}'.format(f)
        line += '\n'
        line += 'gamma'
        for g in gamma_vec:
            line += ',{:0.10f}'.format(g)
        line += '\n'
        line += 'Ngen,{:d}\n'.format(Ngen[theta])
        line += 'Nitr,{:d}\n'.format(Nitr[theta])
        line += 'sample_size,{:d}\n'.format(sample_size)
        line += 'Npar,{:d}\n'.format(Npar)
        line += 'Nepoch,{:d}\n'.format(Nepoch)
        line += 'top_frac,{:0.1f}\n'.format(top_frac)
        line += 'scenario,{}\n'.format(scenario)
        line += 'Npop_lo,{:d}\n'.format(Npop_lo)
        line += 'Npop_hi,{:d}\n'.format(Npop_hi)
        line += 'Ngen_lo,{:d}\n'.format(Ngen_lo[theta])
        line += 'Ngen_hi,{:d}\n'.format(Ngen_hi[theta])
        line += 'write_pop,{:d}\n'.format(write_pop[theta])
        
        sb.call('touch param.txt', shell=True, cwd=dir_path)
        with open(os.path.join(dir_path, 'param.txt'), 'w') as f:
            f.write(line)
        
        #=== partitions ====
        line = str()
        for p in partition_repr(sample_size):
            line += p[1:-1] + '\n'
        sb.call('touch partitions.txt', shell=True, cwd=dir_path)
        with open(os.path.join(dir_path, 'partitions.txt'), 'w') as f:
            f.write(line)
        
        # === bash ====
        if run_on_cluster:
            bash_name = 'run.sh'
            sb.call('touch {}'.format(bash_name), shell=True, cwd=dir_path)

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
./{exec_name}""".format(runtime=runtime[theta], exec_name=exec_name, node=node)

            # write bash file
            with open(os.path.join(dir_path, bash_name), 'w') as f:
                f.write(line)
        
        # === run ===
        if run_on_cluster:
            sb.call('sbatch {}'.format(bash_name), shell=True, cwd=dir_path)
        else:
            sb.call('./{}'.format(exec_name), shell=True, cwd=dir_path)

# clean up
if os.path.isfile('makefile'):
    sb.call('make clean', shell=True, cwd = os.getcwd())
    os.remove('makefile')
