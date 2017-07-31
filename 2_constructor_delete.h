class ClassA {
 public:
	ClassA();
	~ClassA();
	ClassA(ClassA const&)=delete;               // Copy construct
	ClassA(ClassA&&)=delete;                    // Move construct
	ClassA& operator=(ClassA const&)=delete;    // Copy assign
	ClassA& operator=(ClassA &&)=delete;        // Move assign
	int A;
};