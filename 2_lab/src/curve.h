class Nephroid {
   private:
    double _r;
    char* _repr;
    int _repr_len;

   public:
    explicit Nephroid();
    explicit Nephroid(double r);
    explicit Nephroid(const Nephroid &obj) noexcept;
    Nephroid& operator=(const Nephroid &obj) noexcept;
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
