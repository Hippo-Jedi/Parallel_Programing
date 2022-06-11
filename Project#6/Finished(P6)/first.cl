kernel
void
ArrayMult( global const float *dA, global const float *dB, global float *dC )
{
    //1 for multiply/Mult-Reduction, 2 for multiply
    int i = 1;
	int gid = get_global_id( 0 );
    if (i == 1) {
        dC[gid] = dA[gid] * dB[gid];
    }
    if (i == 2) {
        dC[gid] = dA[gid] * dB[gid] + dC[gid];
    }
}