package com.java.linpack;

import android.app.Activity;
import android.app.AlertDialog;
import android.os.Bundle;
import android.widget.TextView;

public class JavaLinpackActivity extends Activity {
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        TextView tv = new TextView(this);
        setContentView(tv);
        tv.setText(Linpack.bench());
        
        new AlertDialog.Builder(this).setMessage("DONE!").show();
    }
    
    
}

/*
**
** LINPACK.C        Linpack benchmark, calculates FLOPS.
**                  (FLoating Point Operations Per Second)
**
** Translated to C by Bonnie Toy 5/88
**
** Modified by Will Menninger, 10/93, with these features:
**  (modified on 2/25/94  to fix a problem with daxpy  for
**   unequal increments or equal increments not equal to 1.
**     Jack Dongarra)
**
** - Defaults to double precision.
** - Averages ROLLed and UNROLLed performance.
** - User selectable array sizes.
** - Automatically does enough repetitions to take at least 10 CPU seconds.
** - Prints machine precision.
** - ANSI prototyping.
**
** To compile:  cc -O -o linpack linpack.c -lm
**
** Now ported to Java by Alexander Samuelsson, 23 years later, 6/11
**
*/

class Linpack {

	final static String PREC = "Double";
	final static double ZERO = 0.0f; //this must be changed to a double when testing DP performance
	final static double ONE = 1.0f; //this must be changed to a double when testing DP performance
	static long g_startTime;
	static String msg = "";

//	public static void main(String[] args){
//		bench();
//	}

	/* Run the benchmark */
	public static String bench() {

		int     arsize;
		long    arsize2d,nreps;
		g_startTime = System.currentTimeMillis();
		arsize = 200;
		msg = msg.concat("\n\nLINPACK benchmark, " + PREC + " precision.\n");
		//System.out.print("Machine precision:  %d digits.\n",BASE10DIG);
		msg = msg.concat("Array size " + arsize + " X " + arsize + "\n");
		msg = msg.concat("Average rolled and unrolled performance:\n\n");
		msg = msg.concat("    Reps Time(s) DGEFA   DGESL  OVERHEAD    MFLOPS\n");
		msg = msg.concat("----------------------------------------------------\n");

		while (true)
		{
			/*System.out.print("Enter array size (q to quit) [200]:  ");
			fgets(buf,79,stdin);
			if (buf[0]=='q' || buf[0]=='Q')
			break;
			if (buf[0]=='\0' || buf[0]=='\n')
			arsize=200;
			else
				arsize=atoi(buf);
				arsize/=2;
				arsize*=2;*/
			if (arsize<10)
			{
				System.out.print("Too small.\n");
			}
			arsize2d = arsize*arsize;
			//memreq=arsize2d*sizeof(float)+(long)arsize*sizeof(float)+(long)arsize*sizeof(int);
			//System.out.print("Memory required:  %ldK.\n",(memreq+512L)>>10);
			//malloc_arg=(size_t)memreq;
			//if (malloc_arg!=memreq || (mempool=malloc(malloc_arg))==NULL)
			//{
			//	System.out.print("Not enough memory available for given array size.\n\n");
			//	continue;
			//}
			nreps=1;
			//linpack_f(nreps,arsize); //ONLY CALL IT ONCE WHEN WE PRINT EVERYTHING
			while (linpack_f(nreps,arsize)<10.) //TODO change back to 10 seconds
			    nreps*=2;
			//free(mempool);
			System.out.print("\n");
			break; //TODO remove
		}
		return msg;
	}

	static void printA(float a[],int lda,int arsize) {
		for(int i = 0; i < arsize; ++i){
			System.out.println();
			for(int j = 0; j < arsize; ++j)
				System.out.print(a[lda*i+j] + " ");
		}
	}

	static double linpack_f(long nreps,int arsize) {
		double  a[], b[];
		double   norma,t1,kflops,tdgesl,tdgefa,totalt,toverhead,ops;
		int   ipvt[],n,info,lda;
		long   i,arsize2d;

		lda = arsize;
		n = arsize/2;
		arsize2d = (long)arsize*(long)arsize;
		ops=((2.0f*n*n*n)/3.0f+2.0f*n*n);
		a= new double[arsize*arsize];
		b= new double[arsize];	//TODO added by me
		//b=a+arsize2d; 			//TODO Think about this
		//ipvt=(int *)&b[arsize];
		ipvt = new int[arsize]; //TODO these 2 lines added by me to stop javas whine about possibly uninitialized vars
		info = 0;
		
		tdgesl=0;
		tdgefa=0;
		totalt=second();
		for (i=0;i<nreps;i++)
		{
			matgen(a,lda,n,b);
			t1 = second();
			dgefa(a,lda,n,ipvt,info,1);
			tdgefa += second()-t1;
			t1 = second();
			dgesl(a,lda,n,ipvt,b,0,1);
			tdgesl += second()-t1;
		}
		for (i=0;i<nreps;i++)
		{
			matgen(a,lda,n,b);
			t1 = second();
			dgefa(a,lda,n,ipvt,info,0);
			tdgefa += second()-t1;
			t1 = second();
			dgesl(a,lda,n,ipvt,b,0,0);
			tdgesl += second()-t1;
		}
		totalt=second()-totalt;
		if (totalt<0.5f || tdgefa+tdgesl<0.2f)
			return(0.f);
		kflops=2.f*nreps*ops/(1000.f*(tdgefa+tdgesl));
		toverhead=totalt-tdgefa-tdgesl;
		if (tdgefa<0.)
			tdgefa=0.f;
		if (tdgesl<0.)
			tdgesl=0.f;
		if (toverhead<0.)
			toverhead=0.f;
		msg = msg.concat(nreps + " " + totalt + " " + 100.f*tdgefa/totalt + " " + 100.f*tdgesl/totalt + " " + 100.f*toverhead/totalt + " " + kflops/1000.0f + "\n");
		//printA(a,lda,arsize); //TODO ADDED BY ME FOR TESTING PURPOSES
		return(totalt);
	}



	/*
	 ** For matgen,
	 ** We would like to declare a[][lda], but c does not allow it.  In this
		 ** function, references to a[i][j] are written a[lda*i+j].
			 */
	static void matgen(double a[], int lda, int n, double b[]) {
		int init,i,j;

		init = 1325;
		double norma = 0.0f;
		for (j = 0; j < n; j++)
			for (i = 0; i < n; i++)
		{
			init = (int)((long)3125*(long)init % 65536L);
			a[lda*j+i] = (init - 32768.0f)/16384.0f;
			norma = (a[lda*j+i] > norma) ? a[lda*j+i] : norma;
		}
		for (i = 0; i < n; i++)
			b[i] = 0.0f;
		for (j = 0; j < n; j++)
			for (i = 0; i < n; i++)
			b[i] = b[i] + a[lda*j+i];

		/* ADDED BY ME print the b array */
		//System.out.print("\nB ARRAY:\n");
		//for(i = 0; i < n; ++i){
		//	System.out.print(b[i] + " ");
		//}

		/* ADDED BY ME print the a array (matrix) */
		//System.out.print("\nA ARRAY:");
		//for (j = 0; j < n; j++){
		//	System.out.print("\n");
		//	for (i = 0; i < n; i++)
		//		System.out.print(a[lda*j+i] + " ");
		//}
	}

	/*
	 **
	 ** DGEFA benchmark
	 **
	 ** We would like to declare a[][lda], but c does not allow it.  In this
		 ** function, references to a[i][j] are written a[lda*i+j].
			 **
	 **   dgefa factors a double precision matrix by gaussian elimination.
	 **
	 **   dgefa is usually called by dgeco, but it can be called
	 **   directly with a saving in time if  rcond  is not needed.
	 **   (time for dgeco) = (1 + 9/n)*(time for dgefa) .
		 **
	 **   on entry
	 **
	 **      a       float precision[n][lda]
	 **              the matrix to be factored.
	 **
	 **      lda     integer
	 **              the leading dimension of the array  a .
	 **
	 **      n       integer
	 **              the order of the matrix  a .
	 **
	 **   on return
	 **
	 **      a       an upper triangular matrix and the multipliers
	 **              which were used to obtain it.
	 **              the factorization can be written  a = l*u  where
	 **              l  is a product of permutation and unit lower
	 **              triangular matrices and  u  is upper triangular.
	 **
	 **      ipvt    integer[n]
	 **              an integer vector of pivot indices.
	 **
	 **      info    integer
	 **              = 0  normal value.
	 **              = k  if  u[k][k] .eq. 0.0 .  this is not an error
		 **                   condition for this subroutine, but it does
	 **                   indicate that dgesl or dgedi will divide by zero
	 **                   if called.  use  rcond  in dgeco for a reliable
	 **                   indication of singularity.
	 **
	 **   linpack. this version dated 08/14/78 .
	 **   cleve moler, university of New Mexico, argonne national lab.
	 **
	 **   functions
	 **
	 **   blas daxpy,dscal,idamax
	 **
	 */
	static void dgefa(double a[],int lda,int n,int ipvt[],int info,int roll)

	{
		double t;
		int j,k,kp1,l,nm1;

		/* gaussian elimination with partial pivoting */

		if (roll != 0)
		{
			info = 0;
			nm1 = n - 1;
			if (nm1 >=  0)
				for (k = 0; k < nm1; k++)
			{
				kp1 = k + 1;

				/* find l = pivot index */

				l = idamax(n-k,a,lda*k+k,1) + k;
				ipvt[k] = l;

				/* zero pivot implies this column already
				 triangularized */

				if (a[lda*k+l] != ZERO)
				{

					/* interchange if necessary */

					if (l != k)
					{
						t = a[lda*k+l];
						a[lda*k+l] = a[lda*k+k];
						a[lda*k+k] = t;
					}

					/* compute multipliers */

					t = -ONE/a[lda*k+k];
					dscal_r(n-(k+1),t,a,lda*k+k+1,1);

					/* row elimination with column indexing */

					for (j = kp1; j < n; j++)
					{
						t = a[lda*j+l];
						if (l != k)
						{
							a[lda*j+l] = a[lda*j+k];
							a[lda*j+k] = t;
						}
						daxpy_r(n-(k+1),t,a,lda*k+k+1,1,a,lda*j+k+1,1);
					}
				}
				else
					(info) = k;
			}
			ipvt[n-1] = n-1;
			if (a[lda*(n-1)+(n-1)] == ZERO)
				(info) = n-1;
		}
		else
		{
			info = 0;
			nm1 = n - 1;
			if (nm1 >=  0)
				for (k = 0; k < nm1; k++)
			{
				kp1 = k + 1;

				/* find l = pivot index */

				l = idamax(n-k,a,lda*k+k,1) + k;
				ipvt[k] = l;

				/* zero pivot implies this column already
				 triangularized */

				if (a[lda*k+l] != ZERO)
				{

					/* interchange if necessary */

					if (l != k)
					{
						t = a[lda*k+l];
						a[lda*k+l] = a[lda*k+k];
						a[lda*k+k] = t;
					}

					/* compute multipliers */

					t = -ONE/a[lda*k+k];
					dscal_ur(n-(k+1),t,a,lda*k+k+1,1);

					/* row elimination with column indexing */

					for (j = kp1; j < n; j++)
					{
						t = a[lda*j+l];
						if (l != k)
						{
							a[lda*j+l] = a[lda*j+k];
							a[lda*j+k] = t;
						}
						daxpy_ur(n-(k+1),t,a,lda*k+k+1,1,a,lda*j+k+1,1);
					}
				}
				else
					(info) = k;
			}
			ipvt[n-1] = n-1;
			if (a[lda*(n-1)+(n-1)] == ZERO)
				(info) = n-1;
		}
	}

	/*
**
** DGESL benchmark
**
** We would like to declare a[][lda], but c does not allow it.  In this
** function, references to a[i][j] are written a[lda*i+j].
**
**   dgesl solves the double precision system
**   a * x = b  or  trans(a) * x = b
**   using the factors computed by dgeco or dgefa.
**
**   on entry
**
**      a       double precision[n][lda]
**              the output from dgeco or dgefa.
**
**      lda     integer
**              the leading dimension of the array  a .
**
**      n       integer
**              the order of the matrix  a .
**
**      ipvt    integer[n]
**              the pivot vector from dgeco or dgefa.
**
**      b       double precision[n]
**              the right hand side vector.
**
**      job     integer
**              = 0         to solve  a*x = b ,
**              = nonzero   to solve  trans(a)*x = b  where
**                          trans(a)  is the transpose.
**
**  on return
**
**      b       the solution vector  x .
**
**   error condition
**
**      a division by zero will occur if the input factor contains a
**      zero on the diagonal.  technically this indicates singularity
**      but it is often caused by improper arguments or improper
**      setting of lda .  it will not occur if the subroutines are
**      called correctly and if dgeco has set rcond .gt. 0.0
**      or dgefa has set info .eq. 0 .
**
**   to compute  inverse(a) * c  where  c  is a matrix
**   with  p  columns
**         dgeco(a,lda,n,ipvt,rcond,z)
**         if (!rcond is too small){
**              for (j=0,j<p,j++)
**                      dgesl(a,lda,n,ipvt,c[j][0],0);
**         }
**
**   linpack. this version dated 08/14/78 .
**   cleve moler, university of new mexico, argonne national lab.
**
**   functions
**
**   blas daxpy,ddot
*/
static void dgesl(double a[],int lda,int n,int ipvt[],double b[],int job,int roll) {
    double    t;
    int     k,kb,l,nm1;

    if (roll != 0)
        {
        nm1 = n - 1;
        if (job == 0)
            {

            /* job = 0 , solve  a * x = b   */
            /* first solve  l*y = b         */

            if (nm1 >= 1)
                for (k = 0; k < nm1; k++)
                    {
                    l = ipvt[k];
                    t = b[l];
                    if (l != k)
                        {
                        b[l] = b[k];
                        b[k] = t;
                        }
                    daxpy_r(n-(k+1),t,a,lda*k+k+1,1,b,k+1,1);
                    }

            /* now solve  u*x = y */

            for (kb = 0; kb < n; kb++)
                {
                k = n - (kb + 1);
                b[k] = b[k]/a[lda*k+k];
                t = -b[k];
                daxpy_r(k,t,a,lda*k+0,1,b,0,1);
                }
            }
        else
            {

            /* job = nonzero, solve  trans(a) * x = b  */
            /* first solve  trans(u)*y = b             */

            for (k = 0; k < n; k++)
                {
                t = ddot_r(k,a,lda*k+0,1,b,0,1);
                b[k] = (b[k] - t)/a[lda*k+k];
                }

            /* now solve trans(l)*x = y     */

            if (nm1 >= 1)
                for (kb = 1; kb < nm1; kb++)
                    {
                    k = n - (kb+1);
                    b[k] = b[k] + ddot_r(n-(k+1),a,lda*k+k+1,1,b,k+1,1);
                    l = ipvt[k];
                    if (l != k)
                        {
                        t = b[l];
                        b[l] = b[k];
                        b[k] = t;
                        }
                    }
            }
        }
    else
        {
        nm1 = n - 1;
        if (job == 0)
            {

            /* job = 0 , solve  a * x = b   */
            /* first solve  l*y = b         */

            if (nm1 >= 1)
                for (k = 0; k < nm1; k++)
                    {
                    l = ipvt[k];
                    t = b[l];
                    if (l != k)
                        {
                        b[l] = b[k];
                        b[k] = t;
                        }
                    daxpy_ur(n-(k+1),t,a,lda*k+k+1,1,b,k+1,1);
                    }

            /* now solve  u*x = y */

            for (kb = 0; kb < n; kb++)
                {
                k = n - (kb + 1);
                b[k] = b[k]/a[lda*k+k];
                t = -b[k];
                daxpy_ur(k,t,a,lda*k+0,1,b,0,1);
                }
            }
        else
            {

            /* job = nonzero, solve  trans(a) * x = b  */
            /* first solve  trans(u)*y = b             */

            for (k = 0; k < n; k++)
                {
                t = ddot_ur(k,a,lda*k+0,1,b,0,1);
                b[k] = (b[k] - t)/a[lda*k+k];
                }

            /* now solve trans(l)*x = y     */

            if (nm1 >= 1)
                for (kb = 1; kb < nm1; kb++)
                    {
                    k = n - (kb+1);
                    b[k] = b[k] + ddot_ur(n-(k+1),a,lda*k+k+1,1,b,k+1,1);
                    l = ipvt[k];
                    if (l != k)
                        {
                        t = b[l];
                        b[l] = b[k];
                        b[k] = t;
                        }
                    }
            }
        }
    }

	/*
	 ** Constant times a vector plus a vector.
	 ** Jack Dongarra, linpack, 3/11/78.
	 ** ROLLED version
	 */
	static void daxpy_r(int n,double da,double dx[],int dxpos,int incx,double dy[],int dypos,int incy) { //TODO dxpos dypos added by me, is the starting index for dx and dy-arrays
		int i,ix,iy;

		if (n <= 0)
			return;
		if (da == ZERO)
			return;

		if (incx != 1 || incy != 1)
		{

			/* code for unequal increments or equal increments != 1 */

			ix = 1; //TODO not sure about these 2 lines
			iy = 1;
			if(incx < 0) ix = (-n+1)*incx + 1;
			if(incy < 0)iy = (-n+1)*incy + 1;
			for (i = 0;i < n; i++)
			{
				dy[iy+dypos] = dy[iy+dypos] + da*dx[ix+dxpos];
				ix = ix + incx;
				iy = iy + incy;
			}
			return;
		}

		/* code for both increments equal to 1 */

		for (i = 0;i < n; i++)
			dy[i+dypos] = dy[i+dypos] + da*dx[i+dxpos];
	}

	/*
	 ** constant times a vector plus a vector.
	 ** Jack Dongarra, linpack, 3/11/78.
	 ** UNROLLED version
	 */
	static void daxpy_ur(int n,double da,double dx[],int dxpos,int incx,double dy[],int dypos,int incy) {
		int i,ix,iy,m;

		if (n <= 0)
			return;
		if (da == ZERO)
			return;

		if (incx != 1 || incy != 1)
		{

			/* code for unequal increments or equal increments != 1 */

			ix = 0;
			iy = 0;
			if(incx < 0) ix = (-n+1)*incx + 1;
			if(incy < 0)iy = (-n+1)*incy + 1;
			for (i = 0;i < n; i++)
			{
				dy[iy+dypos] = dy[iy+dypos] + da*dx[ix+dxpos];
				ix = ix + incx;
				iy = iy + incy;
			}
			return;
		}

		/* code for both increments equal to 1 */

		m = n % 4;
		if ( m != 0)
		{
			for (i = 0; i < m; i++)
				dy[i+dypos] = dy[i+dypos] + da*dx[i+dxpos];
			if (n < 4)
				return;
		}
		for (i = m; i < n; i = i + 4)
		{
			dy[i+dypos] = dy[i+dypos] + da*dx[i+dxpos];
			dy[i+1+dypos] = dy[i+1+dypos] + da*dx[i+1+dxpos];
			dy[i+2+dypos] = dy[i+2+dypos] + da*dx[i+2+dxpos];
			dy[i+3+dypos] = dy[i+3+dypos] + da*dx[i+3+dxpos];
		}
	}

	/*
	 ** Finds the index of element having max. absolute value.
	 ** Jack Dongarra, linpack, 3/11/78.
	 */
	static int idamax(int n,double dx[], int dxpos,int incx) { //TODO The dxpos is added by me and is the starting index of dx[] NEED A DOUBLE version of this
		double dmax;
		int i, ix, itemp;

		itemp = 0; //ADDED BY ME

		if (n < 1)
			return(-1);
		if (n ==1 )
			return(0);
		if(incx != 1)
		{

			/* code for increment not equal to 1 */

			ix = 0; //TODO dxpos or dxpos+1?
			dmax = Math.abs((double)dx[0]);
			ix = ix + incx;
			for (i = 1; i < n; i++)
			{
				if(Math.abs((double)dx[ix]) > dmax)
				{
					itemp = i;
					dmax = Math.abs((double)dx[ix+dxpos]);
				}
				ix = ix + incx;
			}
		}
		else
		{

			/* code for increment equal to 1 */

			itemp = 0;
			dmax = Math.abs((double)dx[0]);
			for (i = 1; i < n; i++)
				if(Math.abs((double)dx[i+dxpos]) > dmax)
			{
				itemp = i;
				dmax = Math.abs((double)dx[i+dxpos]);
			}
		}
		return (itemp);
	}

/*
** Forms the dot product of two vectors.
** Jack Dongarra, linpack, 3/11/78.
** ROLLED version
*/
static double ddot_r(int n,double dx[],int dxpos,int incx,double dy[],int dypos,int incy) {
    double dtemp;
    int i,ix,iy;

    dtemp = ZERO;

    if (n <= 0)
        return(ZERO);

    if (incx != 1 || incy != 1)
        {

        /* code for unequal increments or equal increments != 1 */

        ix = 0; //TODO These 2 lines edited by me
        iy = 0;
        if (incx < 0) ix = (-n+1)*incx;
        if (incy < 0) iy = (-n+1)*incy;
        for (i = 0;i < n; i++)
            {
            dtemp = dtemp + dx[ix+dxpos]*dy[iy+dypos];
            ix = ix + incx;
            iy = iy + incy;
            }
        return(dtemp);
        }

    /* code for both increments equal to 1 */

    for (i=0;i < n; i++)
        dtemp = dtemp + dx[i]*dy[i];
    return(dtemp);
    }

/*
** Forms the dot product of two vectors.
** Jack Dongarra, linpack, 3/11/78.
** UNROLLED version
*/
static double ddot_ur(int n,double dx[],int dxpos,int incx,double dy[],int dypos,int incy) {
    double dtemp;
    int i,ix,iy,m;

    dtemp = ZERO;

    if (n <= 0)
        return(ZERO);

    if (incx != 1 || incy != 1)
        {

        /* code for unequal increments or equal increments != 1 */

        ix = 0;
        iy = 0;
        if (incx < 0) ix = (-n+1)*incx;
        if (incy < 0) iy = (-n+1)*incy;
        for (i = 0;i < n; i++)
            {
            dtemp = dtemp + dx[ix+dxpos]*dy[iy+dypos];
            ix = ix + incx;
            iy = iy + incy;
            }
        return(dtemp);
        }

    /* code for both increments equal to 1 */

    m = n % 5;
    if (m != 0)
        {
        for (i = 0; i < m; i++)
            dtemp = dtemp + dx[i]*dy[i];
        if (n < 5)
            return(dtemp);
        }
    for (i = m; i < n; i = i + 5)
        {
        dtemp = dtemp + dx[i]*dy[i] +
        dx[i+1+dxpos]*dy[i+1+dypos] + dx[i+2+dxpos]*dy[i+2+dypos] +
        dx[i+3+dxpos]*dy[i+3+dypos] + dx[i+4+dxpos]*dy[i+4+dypos];
        }
    return(dtemp);
    }
	
	/*
	 ** Scales a vector by a constant.
	 ** Jack Dongarra, linpack, 3/11/78.
	 ** UNROLLED version
	 */
	static void dscal_ur(int n,double da,double dx[],int dxpos,int incx) {
				int i,m,nincx;

				if (n <= 0)
					return;
				if (incx != 1)
				{

		/* code for increment not equal to 1 */

					nincx = n*incx;
					for (i = 0; i < nincx; i = i + incx) //TODO not sure about i = dxpos
						dx[i+dxpos] = da*dx[i+dxpos];
					return;
				}

		/* code for increment equal to 1 */

				m = n % 5;
				if (m != 0)
				{
					for (i = 0; i < m; i++) 
						dx[i+dxpos] = da*dx[i+dxpos]; //TODO not sure about i+dxpos
					if (n < 5)
						return;
				}
				for (i = m; i < n; i = i + 5)
				{
				dx[i+dxpos] = da*dx[i+dxpos];
					dx[i+1+dxpos] = da*dx[i+1+dxpos];
					dx[i+2+dxpos] = da*dx[i+2+dxpos];
					dx[i+3+dxpos] = da*dx[i+3+dxpos];
					dx[i+4+dxpos] = da*dx[i+4+dxpos];
				}
	}

	/*
	 ** Scales a vector by a constant.
	 ** Jack Dongarra, linpack, 3/11/78.
	 ** ROLLED version
	 */
	static void dscal_r(int n,double da,double dx[],int dxpos,int incx) { //TODO do we need dxpos here?
		int i,nincx;

				if (n <= 0)
					return;
				if (incx != 1)
				{
		
		/* code for increment not equal to 1 */

					nincx = n*incx;
					for (i = 0; i < nincx; i = i + incx) 
						dx[i+dxpos] = da*dx[i+dxpos];		//TODO NOT SURE ABOUT i+dxpos
					return;
				}

		/* code for increment equal to 1 */

				for (i = 0; i < n; i++)				
					dx[i+dxpos] = da*dx[i+dxpos];			//TODO NOT SURE ABOUT i+dxpos
	}

	//TODO need a double prec version for this
	static double second() {
		return (System.currentTimeMillis() - g_startTime)/1000.0f;
	}


}