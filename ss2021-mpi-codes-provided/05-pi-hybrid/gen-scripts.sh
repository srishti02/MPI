#!/bin/bash
BASENAME=pi-hybrid
TIME=0-00:02
NTASKS=3
CPUS_PER_TASK=2
MEM_PER_CPU=500M

function gen_run_script()
{
  local EXECUTABLE="$1"
  local SCRIPT="$2"

  cat >"${SCRIPT}" <<ZZEOF
#!/bin/bash
echo 'OMP_THREAD_LIMIT=${CPUS_PER_TASK} mpirun --bind-to none -np ${NTASKS} ./${EXECUTABLE}'
OMP_THREAD_LIMIT=${CPUS_PER_TASK} mpirun --bind-to none -np ${NTASKS} ./${EXECUTABLE}
ZZEOF
  chmod +x "${SCRIPT}"
}

function gen_sbatch_script()
{
  local EXECUTABLE="$1"
  local SCRIPT="$2"

  cat >"${SCRIPT}" <<ZZEOF
#!/bin/bash
#SBATCH --time ${TIME}
#SBATCH --ntasks=${NTASKS}
#SBATCH --cpus-per-task=${CPUS_PER_TASK}
#SBATCH --mem-per-cpu=${MEM_PER_CPU}
export OMP_THREAD_LIMIT=${CPUS_PER_TASK}
srun ./${EXECUTABLE}
ZZEOF
  chmod +x "${SCRIPT}"
}

for l in c cpp fortran ; do
  gen_run_script ${BASENAME}-${l}.exe run-${l}.sh
  gen_sbatch_script ${BASENAME}-${l}.exe sbatchjob-${l}.sh
done

exit 0
