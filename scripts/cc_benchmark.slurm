#!/bin/bash
#SBATCH --time=02:20:00
#SBATCH --nodes=1
#SBATCH --job-name="mp0-benchmark"
#SBATCH --output=mp0-benchmark.%J.out
#SBATCH --error=mp0-benchmark.%J.err
#SBATCH -p secondary

#moves to the directory the user was in when they ran qsub
cd $SLURM_SUBMIT_DIR #assumed to be the source tree

#creates an out-of-tree build directory for CMake and moves to it
mkdir -p $SLURM_SUBMIT_DIR/build
pushd $SLURM_SUBMIT_DIR/build

#get cpuinfo
cat /proc/cpuinfo > $SLURM_SUBMIT_DIR/writeup/cpuinfo.txt

#build the programs (into the build directory, IE, the current directory)
#then benchmark them. Quit early on failure. Run all of them under singularity.
srun --mpi=pmi2 --ntasks 1 /projects/cs/cs484/sing_exec.sh cmake ${SLURM_SUBMIT_DIR} \
&& srun --mpi=pmi2 --ntasks 1 /projects/cs/cs484/sing_exec.sh make \
&& echo "Benchmarking" \
&& srun --mpi=pmi2 --ntasks 1 /projects/cs/cs484/sing_exec.sh ./bench/bench2 --benchmark_out_format=json --benchmark_out=${SLURM_SUBMIT_DIR}/gbench_out.json > ${SLURM_SUBMIT_DIR}/gbench_out.txt

#return to the original directory
popd
