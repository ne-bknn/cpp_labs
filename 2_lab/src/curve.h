class Nephroid {
   private:
    double _r;
    char* _repr;

   public:
    explicit Nephroid(double r);
    ~Nephroid();
    double r();
    double R();

    void set_r(double r);

    double arclength();
    double area();
    double curvature_radius(double t);
    double x(double t);
    double y(double t);

    char* repr();
};
