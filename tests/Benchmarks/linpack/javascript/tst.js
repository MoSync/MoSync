
PREC = "Double";
ZERO = 0.0; //this must be changed to a double when testing DP performance
ONE = 1.0; //this must be changed to a double when testing DP performance
var g_startTime; 

function bench() {
	g_startTime = (new Date).getTime(); //start the timer
	arsize = 200;
	msg = "LINPACK benchmark, " + PREC + " precision.<br/>";
	msg += "Array size " + arsize + " X " + arsize + "<br/>";
	msg += "Average rolled and unrolled performance:<br/><br/>";
	msg += "    Reps Time(s) DGEFA   DGESL  OVERHEAD    MFLOPS<br/>";
	msg += "----------------------------------------------------------<br/>";
	nreps = 1;
	
	while (linpack_f(nreps,arsize)<5000.0) //TODO change back to 10 seconds
		nreps*=2;
	//linpack_f(nreps,arsize); // only call it once when we print debug info
	
	return msg;
}

function linpack_f(nreps, arsize) {
		var  a= new Array(); var b = new Array();
		var  norma,t1,kflops,tdgesl,tdgefa,totalt,toverhead,ops;
		var  ipvt = new Array();
		var  n,info,lda;
		var  i,arsize2d;

		lda = arsize;
		n = arsize/2;
		arsize2d = arsize*arsize;
		ops=((2.0*n*n*n)/3.0+2.0*n*n);
		//a= new double[arsize*arsize];
		//b= new double[arsize];	//TODO added by me
		//b=a+arsize2d; 			//TODO Think about this
		//ipvt=(int *)&b[arsize];
		//ipvt = new int[arsize]; //TODO these 2 lines added by me to stop javas whine about possibly uninitialized vars
		var info = 0;
		
		var tdgesl=0;
		var tdgefa=0;
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
		if (totalt<0.5 || tdgefa+tdgesl<0.2)
			return(0.0);
		kflops=2.0*nreps*ops/(1000.0*(tdgefa+tdgesl));
		toverhead=totalt-tdgefa-tdgesl;
		if (tdgefa<0.0)
			tdgefa=0.0;
		if (tdgesl<0.0)
			tdgesl=0.0;
		if (toverhead<0.0)
			toverhead=0.0;
		msg += nreps + " " + totalt + " " + 100.0*tdgefa/totalt + " " + 100.0*tdgesl/totalt + " " + 100.0*toverhead/totalt + " " + kflops/1000.0 + "<br/>"; //num.toFixed(int) kflops/1000 to get MFLOPS, TODO is the constants correct?!
		//printA(a,lda,arsize); //TODO ADDED BY ME FOR DEBUGING PURPOSES
		return(totalt);
	}
	
	/*
	 ** For matgen,
	 ** We would like to declare a[][lda], but c does not allow it.  In this
	 ** function, references to a[i][j] are written a[lda*i+j].
	 */
	function matgen(a, lda, n, b) {
		var init,i,j;

		init = 1325;
		norma = 0.0;
		for (j = 0; j < n; j++)
			for (i = 0; i < n; i++)
		{
			init = (3125*init % 65536); 
			a[lda*j+i] = (init - 32768.0)/16384.0;
			norma = (a[lda*j+i] > norma) ? a[lda*j+i] : norma; 
		}
		for (i = 0; i < n; i++)
			b[i] = 0.0;
		for (j = 0; j < n; j++)
			for (i = 0; i < n; i++)
			b[i] = b[i] + a[lda*j+i];

		/* ADDED BY ME print the b array */
		//System.out.print("\nB ARRAY:\n");
		//for(i = 0; i < n; ++i){
		//	document.write(b[i] + " ");
		//}

		/* ADDED BY ME print the a array (matrix) */
		//System.out.print("\nA ARRAY:");
		//for (j = 0; j < n; j++){
		//	document.write("<br/>");
		//	for (i = 0; i < n; i++)
		//		document.write(a[lda*j+i] + " ");
		//}
	}

	function dgefa(a, lda, n, ipvt, info, roll)
	{
		var t;
		var j,k,kp1,l,nm1;

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
	function dgesl(a, lda, n, ipvt, b, job, roll) {
		var    t;
		var     k,kb,l,nm1;

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
	 ** Scales a vector by a constant.
	 ** Jack Dongarra, linpack, 3/11/78.
	 ** UNROLLED version
	 */ 
	function dscal_ur(n, da, dx, dxpos, incx) {
				var i,m,nincx;

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
	function dscal_r(n, da, dx, dxpos, incx) { //TODO do we need dxpos here?
		var i,nincx;

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

	
	/*
	 ** constant times a vector plus a vector.
	 ** Jack Dongarra, linpack, 3/11/78.
	 ** UNROLLED version
	 */
	function daxpy_ur(n, da, dx, dxpos, incx, dy, dypos, incy) {
		var i,ix,iy,m;

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
	 ** Constant times a vector plus a vector.
	 ** Jack Dongarra, linpack, 3/11/78.
	 ** ROLLED version
	 */
	function daxpy_r(n, da, dx, dxpos, incx, dy, dypos, incy) { //TODO dxpos dypos added by me, is the starting index for dx and dy-arrays
		var i,ix,iy;

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
	 ** Finds the index of element having max. absolute value.
	 ** Jack Dongarra, linpack, 3/11/78.
	 */
	function idamax(n, dx, dxpos, incx) { //TODO The dxpos is added by me and is the starting index of dx[] NEED A DOUBLE version of this
		var dmax;
		var i, ix, itemp;

		itemp = 0; //ADDED BY ME

		if (n < 1)
			return(-1);
		if (n ==1 )
			return(0);
		if(incx != 1)
		{

			/* code for increment not equal to 1 */

			ix = 0; //TODO dxpos or dxpos+1?
			dmax = Math.abs(dx[0]);
			ix = ix + incx;
			for (i = 1; i < n; i++)
			{
				if(Math.abs(dx[ix]) > dmax)
				{
					itemp = i;
					dmax = Math.abs(dx[ix+dxpos]);
				}
				ix = ix + incx;
			}
		}
		else
		{

			/* code for increment equal to 1 */

			itemp = 0;
			dmax = Math.abs(dx[0]);
			for (i = 1; i < n; i++)
				if(Math.abs(dx[i+dxpos]) > dmax)
			{
				itemp = i;
				dmax = Math.abs(	dx[i+dxpos]);
			}
		}
		return (itemp);
	}

/* 
 * Return time passed since g_startTime in milliseconds
 * Named second() because that was the original name in linpack.c and 
 * I want the naming to match.
 */	
function second() {
	return ((new Date).getTime()) - g_startTime;
}

/*
 * Print the test results to the page
 */
function printRes() {
	document.write("<p>" + bench() + "</p>");
}



