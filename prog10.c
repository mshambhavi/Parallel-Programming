/* MPI program that uses a monte carlo method to compute the value of PI */
#include &lt;stdlib.h&gt;
#include &lt;stdio.h&gt;
#include &lt;math.h&gt;
#include &lt;string.h&gt;
#include &lt;stdio.h&gt;
#include &lt;sprng.h&gt;
#include &lt;mpi.h&gt;
#define USE_MPI
#define SEED 35791246
main(int argc, char *argv[])
{
int niter=0;
double x,y;
int i,j,count=0,mycount; /* # of points in the 1st quadrant of unit
circle */
double z;
double pi;
int myid,numprocs,proc;
MPI_Status status;
int master =0;
int tag = 123;
int *stream_id; /* stream id generated by SPRNGS */
MPI_Init(&amp;argc,&amp;argv);
MPI_Comm_size(MPI_COMM_WORLD,&amp;numprocs);
MPI_Comm_rank(MPI_COMM_WORLD,&amp;myid);
if (argc &lt;=1) {
fprintf(stderr,&quot;Usage: monte_pi_mpi number_of_iterations\n&quot;);
MPI_Finalize();
exit(-1);
}
sscanf(argv[1],&quot;%d&quot;,&amp;niter); /* 1st argument is the number of
iterations*/
/* initialize random numbers */
stream_id = init_sprng(myid,numprocs,SEED,SPRNG_DEFAULT);
mycount=0;
for ( i=0; i&lt;niter; i++) {
x = (double)sprng(stream_id);
y = (double)sprng(stream_id);
z = x*x+y*y;
if (z&lt;=1) mycount++;
}
if (myid ==0) { /* if I am the master process gather results from others
*/
count = mycount;
for (proc=1; proc&lt;numprocs; proc++) {
MPI_Recv(&amp;mycount,1,MPI_REAL,proc,tag,MPI_COMM_WORLD,&amp;status);
count +=mycount;
}
pi=(double)count/(niter*numprocs)*4;
printf(&quot;\n # of trials= %d , estimate of pi is %g
\n&quot;,niter*numprocs,pi);
}
else { /* for all the slave processes send results to the master */

printf(&quot;Processor %d sending results= %d to master
process\n&quot;,myid,mycount
);
MPI_Send(&amp;mycount,1,MPI_REAL,master,tag,MPI_COMM_WORLD);
}
MPI_Finalize(); /* let MPI finish up */
}
