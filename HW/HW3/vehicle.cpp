// Base class Vehicle:
class Vehicle
{
public:
    Vehicle(std::string id):m_id(id){}
    virtual ~Vehicle(){}
    virtual std::string description() const = 0;
    virtual bool canHover() const = 0;
    std::string id() const
    {
        return m_id;
    }
private:
    std::string m_id;
};


// Class Drone:
class Drone : public Vehicle
{
public:
    Drone(std::string id):Vehicle(id){}
    virtual ~Drone()
    {
        std::cout << "Destroying " << id() << ", a drone" << std::endl;
    }
    virtual std::string description() const
    {
        return "a drone";
    }
    virtual bool canHover() const
    {
        return true;
    }
};


// Class Balloon:
class Balloon : public Vehicle
{
public:
    Balloon(std::string id, double size):Vehicle(id), m_size(size){}
    virtual ~Balloon()
    {
        std::cout << "Destroying the balloon " << id() << std::endl;
    }
    virtual std::string description() const
    {
        if(m_size < 8)
        {
            return "a small balloon";
        }
        else
        {
            return "a large balloon";
        }
    }
    virtual bool canHover() const
    {
        return true;
    }
private:
    double m_size;
};


// Class Satellite:
class Satellite : public Vehicle
{
public:
    Satellite(std::string id):Vehicle(id){}
    virtual ~Satellite()
    {
        std::cout << "Destroying the satellite " << id() << std::endl;
    }
    virtual std::string description() const
    {
        return "a satellite";
    }
    virtual bool canHover() const
    {
        return false;
    }
};


void display(const Vehicle* v)
{
    cout << v->id() << " is " << v->description();
    if (v->canHover())
        cout << ", so it can hover";
    cout << endl;
}
