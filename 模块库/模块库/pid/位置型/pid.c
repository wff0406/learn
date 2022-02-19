struct _pid{
	float set_speed;
	float act_speed;
	float err;
	float last_err;
	float kp,ki,kd;
	float voltage;
	float intergal;
}pid;
void pid_init()
{
	printf("pid init start\n");
	pid.set_speed=0.0;
	pid.act_speed=0.0;
	pid.err=0.0;
	pid.last_err=0.0;
	pid.voltage=0.0;
	pid.intergal=0.0;
	pid.kp=0.2;
	pid.ki=0.015;
	pid.kd=0.2;
	printf("pid init end\n");
}
float pid_control(float speed)
{
	pid.set_speed=speed;
	pid.err=pid.set_speed-pid.act_speed;
	pid.intergal+=pid.err;
	pid.voltage=pid.kp*pid.err+pid.ki*pid.intergal+pid.kd*(pid.err-pid.last_err);
	pid.last_err=pid.err;
	pid.act_speed=pid.voltage*1.0;
	return pid.act_speed;
}
int main()
{
	printf("system start\n");
	pid_init();
	int count=0;
	while(count<1000)
	{
	float speed=pid_control(200.0);
	printf("%f\n",speed);
	count++;
}
return 0;
}
