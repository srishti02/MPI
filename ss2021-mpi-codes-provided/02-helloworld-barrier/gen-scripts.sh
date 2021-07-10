#!/bin/bash
BASENAME=helloworld
TIME=0-00:02
NTASKS=4
MEM_PER_CPU=500M

function gen_run_script()
{
  local EXECUTABLE="$1"
  local SCRIPT="$2"

  cat >"${SCRIPT}" <<ZZEOF
#!/bin/bash
mpirun -np ${NTASKS} ./${EXECUTABLE}
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
#SBATCH --cpus-per-task=1
#SBATCH --mem-per-cpu=${MEM_PER_CPU}
srun ./${EXECUTABLE}
ZZEOF
  chmod +x "${SCRIPT}"
}

for l in c cpp fortran ; do
  gen_run_script ${BASENAME}-${l}.exe run-${l}.sh
  gen_sbatch_script ${BASENAME}-${l}.exe sbatchjob-${l}.sh
done

exit 0
