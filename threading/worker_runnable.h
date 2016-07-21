//@file worker_runnable.h

class Work
{
public:
	virtual int operator()()=0;
}


template<class Fountor>
class WorkTmpl
{
public:
	/**
	*f(int i, float j);
	*int a; float b;
	*WorkTmpl w(std::bind(f, a, b));
	*/
	WorkTmpl(Foutor);

private:
	Fountor _func;
};

class WorkerRunnable
{
public:
	void work();
	
	void Post(Work* w);

private:
	list<work_ptr_t> _work_queue;
}
