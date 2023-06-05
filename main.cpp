#include <iostream>
#include<cmath>
#include<vector>
#include<string>
#include<random>
#include<algorithm>
using namespace std;

class Point3d{
    private:
        double x;
        double y;
        double z;
    public:
        Point3d(double x,double y,double z):x(x),y(y),z(z){
        }
        Point3d():x(0),y(0),z(0){
        }
        double getX(){return x;}
        double getY(){return y;}
        double getZ(){return z;}
        void setX(double xNew){x=xNew;}
        void setY(double yNew){y=yNew;}
        void setZ(double zNew){z=zNew;}
        friend std::ostream& operator<< (std::ostream& o, const Point3d& p){
            return o<<"Point3d:X: "<<p.x<<" Y:"<<p.y<<" Z:"<<p.z;
        }

};
class Vector3d{
    private:
        double x;
        double y;
        double z;
    public:
        Vector3d(double x,double y,double z):x(x),y(y),z(z){
        }
        Vector3d():x(0),y(0),z(0){
        }
        double getX(){return x;}
        double getY(){return y;}
        double getZ(){return z;}
        void setX(double xNew){x=xNew;}
        void setY(double yNew){y=yNew;}
        void setZ(double zNew){z=zNew;}
        friend std::ostream& operator<< (std::ostream& o, const Vector3d& v){
            return o<<"Vector3d:X: "<<v.x<<" Y:"<<v.y<<" Z:"<<v.z;
        }

};
class Shape3d{
    public:
        virtual ~Shape3d()=default;
        virtual Point3d getPoint( double t )=0;
        virtual Vector3d getFirstDerivative(double t) =0;
        virtual void print (std::ostream& os) const =0;
        friend std::ostream& operator<< (std::ostream& o, const Shape3d& b);
};
inline std::ostream& operator<< (std::ostream& o, const Shape3d& s)
{
  s.print(o);
  return o;
};

class Circle: public Shape3d{
    private:
        double radius;
    public:
        Circle(double radius):radius(radius){
        }
        double getRadius(){return radius;}
        Point3d getPoint( double t) override{
            Point3d point;
            point.setX(radius*cos(t));
            point.setY(radius*sin(t));
            return point;
        }
        Vector3d getFirstDerivative( double t) override{
            Vector3d v;
            v.setX(radius*-sin(t)); //adding dx/dt
            v.setY(radius*cos(t)); //adding dy/dt
            v.setZ(0); // dz/dt   
            return v;
        }
        void print(std::ostream& os)const override{
            os<<"Circle: radius: "<<radius;
        }
};

class Ellipse:public Shape3d{
    private:
        double radiusX;
        double radiusY;
    public:
        Ellipse(double radiusX,double radiusY):radiusX(radiusX),radiusY(radiusY){
        }
        Point3d getPoint( double t) override{
            Point3d point;
            point.setX(radiusX*cos(t));
            point.setY(radiusY*sin(t));
            return point;
        }
        Vector3d getFirstDerivative( double t) override{
            Vector3d v;
            v.setX(radiusX*-sin(t)); //adding dx/dt
            v.setY(radiusY*cos(t)); //adding dy/dt
            v.setZ(0); // dz/dt   
            return v;
        }
        void print(std::ostream& os)const override{
            os<<"Ellipse: radiusX:"<<radiusX<<" radiusY:"<<radiusY;
        }
};

class Helix :public Shape3d{
    private:
        double radius;
        double step;
    public:
        Helix(double radius, double step):radius(radius),step(step){
        }
        Point3d getPoint( double t) override{
            Point3d point;
            point.setX(radius*cos(t));
            point.setY(radius*sin(t));
            point.setZ(t);
            return point;
        }
        Vector3d getFirstDerivative( double t) override{
            Vector3d v;
            v.setX(radius*-sin(t)); //adding dx/dt
            v.setY(radius*cos(t)); //adding dy/dt
            v.setZ(1); // dz/dt 
            return v;  
        }
        void print(std::ostream& os)const override{
            os<<"Helix: radius: " <<radius<<" step:"<<step;
        }
};
vector<Shape3d *> const generateShapes(int numberOfShapes){
    vector<Shape3d *> v;
    for(int i=0;i<numberOfShapes;i++){
        int res=rand()%3;
        if(res==0){
            v.push_back(new Circle(rand()%10+1)); //random radius between 1 and 10
        }else if(res==1){
            v.push_back(new Ellipse(rand()%10+1,rand()%10+1));//random radiuses betwwen 1 and 10
        }else{
            v.push_back(new Helix(rand()%10+1,(float)(rand()) / (float)(RAND_MAX)*6*M_PI)); //step in (0,6*PI) range
        }
    }
    
    return v;
}
bool cmp(Circle* a,Circle* b){
    return a->getRadius() < b->getRadius();
}
int main(){
    vector<Shape3d *>v=generateShapes(20);
    for(int i=0;i<20;i++){
        cout<<*v[i]<<"\n";
        cout<<v[i]->getPoint(M_PI/4)<<"\n";
        cout<<v[i]->getFirstDerivative(M_PI/4)<<"\n";
        cout<<"____\n";
    }
    vector<Circle *>v2;

    for(int i=0;i<20;i++){
        if(dynamic_cast<Circle*>(v[i])){
            v2.push_back(dynamic_cast<Circle*>(v[i]));
        }
    }
    cout<<"SECOND CONTAINER:\n";
    for(int i=0;i<v2.size();i++){
        cout<<*v2[i]<<"\n";
        cout<<"____\n";
    }
    sort(v2.begin(),v2.end(),cmp);
    cout<<"AFTER SORTING:\n";
    for(int i=0;i<v2.size();i++){
        cout<<*v2[i]<<"\n";
        cout<<"____\n";
    }
    double sum=0;
    for(int i=0;i<v2.size();i++){
        sum+=v2[i]->getRadius();
    }
    cout<<"Total sum:"<<sum;
}