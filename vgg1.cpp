// On linux:
// g++ vgg1.cpp -g -std=c++11 -I ../include -I ../tools -L ../bin -lHalide `libpng-config --cflags --ldflags` -ljpeg -lpthread -ldl -o vgg1
// LD_LIBRARY_PATH=../bin ./lesson_12
// On os x:
// g++ lesson_12*.cpp -g -std=c++11 -I ../include -I ../tools -L ../bin -lHalide `libpng-config --cflags --ldflags` -ljpeg -o lesson_12
// DYLD_LIBRARY_PATH=../bin ./lesson_12
#include "Halide.h"
#include "clock.h"
#include <stdio.h>
#include "halide_image_io.h"
#define XL2 128
#define YL2 128
#define XIN 128
#define YIN 128
#define Tm 16
#define Tn 64
#define Ti 16
#define To 16
#define d 20
#define M 128
#define N 128
#define O 64
#define s1 3*3*64*64
#define s2 64
#define s3 3*3*64






using namespace Halide;
using namespace Halide::Tools;
using namespace std;
void input_f(char *fn,int size,float *output)
{
  FILE *fp = fopen(fn,"r");

  //printf("opened\n");
  int i=0;
  char k;
  do{
    fscanf(fp,"%f",&output[i++]);

     k=fgetc(fp);
  }while(k!=EOF);

  /*for(int i=0;i<size;i++){
      printf("%f\n",output[i]);
  }*/
  fclose(fp);

}

class vgg{
public:
  

    Buffer<uint8_t> in_layer;
  Func f_in_bound;

  Func in_layert={Func("input")}; 
  Func f_conv[d];
  Func weight[d]={Func("weight")};//I added {} here for initializar list
  Func b[d]={Func("bias")};
  Func bconv[d]={Func("boundconv")};
      //  RDom r1(0, 3, 0, 3, 0, O,0,O);
  Var n,m,i,o,c;   
  Func in_layerd;
  Func in_layerb;
  Func convertb={Func("convertb")};
  Func output={Func("output")};
  Func newconvert={Func("nconv")};
  Func mconv={Func("mconv")};
  Func convert2={Func("convert")};
  Func final;
  Func bin={Func("bini")};
  RDom r,ri;
  int pad;


    vgg(Buffer<uint8_t> in) : in_layer(in) {
    static float bias_0[s2];
    static float bias_1[s2];
    static float bias_2[s2];
    static float bias_3[s2];
    static float bias_4[s2];
    static float bias_5[s2];
    static float bias_6[s2];
    static float bias_7[s2];
    static float bias_8[s2];
    static float bias_9[s2];
    static float bias_10[s2];
    static float bias_11[s2];
    static float bias_12[s2];
    static float bias_13[s2];
    static float bias_14[s2];
    static float bias_15[s2];
    static float bias_16[s2];
    static float bias_17[s2];
    static float bias_18[s2];
    static float bias_19[1];
  
    static float weight_0[s3];
    static float weight_1[s1];
    static float weight_2[s1];
    static float weight_3[s1];
    static float weight_4[s1];
    static float weight_5[s1];
    static float weight_6[s1];
    static float weight_7[s1];
    static float weight_8[s1];
    static float weight_9[s1];
    static float weight_10[s1];
    static float weight_11[s1];
    static float weight_12[s1];
    static float weight_13[s1];
    static float weight_14[s1];
    static float weight_15[s1];
    static float weight_16[s1];
    static float weight_17[s1];
    static float weight_18[s1];
    static float weight_19[s3];
    input_f("bias_0.dat",s2,bias_0);
    input_f("bias_1.dat",s2,bias_1);
    input_f("bias_2.dat",s2,bias_2);
    input_f("bias_3.dat",s2,bias_3);
    input_f("bias_4.dat",s2,bias_4);
    input_f("bias_5.dat",s2,bias_5);
    input_f("bias_6.dat",s2,bias_6);
    input_f("bias_7.dat",s2,bias_7);
    input_f("bias_8.dat",s2,bias_8);
    input_f("bias_9.dat",s2,bias_9);
    input_f("bias_10.dat",s2,bias_10);
    input_f("bias_11.dat",s2,bias_11);
    input_f("bias_12.dat",s2,bias_12);
    input_f("bias_13.dat",s2,bias_13);
    input_f("bias_14.dat",s2,bias_14);
    input_f("bias_15.dat",s2,bias_15);
    input_f("bias_16.dat",s2,bias_16);
    input_f("bias_17.dat",s2,bias_17);
    input_f("bias_18.dat",s2,bias_18);
    input_f("bias_19.dat",1,bias_19);




    input_f("weight_0.dat",s3,weight_0);
    input_f("weight_1.dat",s1,weight_1);
    input_f("weight_2.dat",s1,weight_2);
    input_f("weight_3.dat",s1,weight_3);
    input_f("weight_4.dat",s1,weight_4);
    input_f("weight_5.dat",s1,weight_5);
    input_f("weight_6.dat",s1,weight_6);
    input_f("weight_7.dat",s1,weight_7);
    input_f("weight_8.dat",s1,weight_8);
    input_f("weight_9.dat",s1,weight_9);
    input_f("weight_10.dat",s1,weight_10);
    input_f("weight_11.dat",s1,weight_11);
    input_f("weight_12.dat",s1,weight_12);
    input_f("weight_13.dat",s1,weight_13);
    input_f("weight_14.dat",s1,weight_14);
    input_f("weight_15.dat",s1,weight_15);
    input_f("weight_16.dat",s1,weight_16);
    input_f("weight_17.dat",s1,weight_17);
    input_f("weight_18.dat",s1,weight_18);
    input_f("weight_19.dat",s3,weight_19);
          //  int pad = 1; // padding  required to handle boundaries

        f_in_bound = BoundaryConditions::constant_exterior(in_layer,//source
                0,//value
                0,in_layer.width(),//bounds
                0,in_layer.height()//bounds
                );//, 128,//padding 0 on the boundary


    Buffer<float> W0(weight_0,3, 3,1, 64), b0(bias_0,64);//3*3*64 single picture mapped into 64 channels
    Buffer<float> W1(weight_1,3, 3, 64,64), b1(bias_1,64);
    Buffer<float> W2(weight_2,3, 3, 64,64), b2(bias_2,64);
    Buffer<float> W3(weight_3,3, 3, 64,64), b3(bias_3,64);
    Buffer<float> W4(weight_4,3, 3, 64,64), b4(bias_4,64);
    Buffer<float> W5(weight_5,3, 3, 64,64), b5(bias_5,64);
    Buffer<float> W6(weight_6,3, 3, 64,64), b6(bias_6,64);
    Buffer<float> W7(weight_7,3, 3, 64,64), b7(bias_7,64);
    Buffer<float> W8(weight_8,3, 3, 64,64), b8(bias_8,64);
    Buffer<float> W9(weight_9,3, 3, 64,64), b9(bias_9,64);
    Buffer<float> W10(weight_10,3, 3, 64,64), b10(bias_10,64);
    Buffer<float> W11(weight_11,3, 3, 64,64), b11(bias_11,64);
    Buffer<float> W12(weight_12,3, 3, 64,64), b12(bias_12,64);
    Buffer<float> W13(weight_13,3, 3, 64,64), b13(bias_13,64);
    Buffer<float> W14(weight_14,3, 3, 64,64), b14(bias_14,64);
    Buffer<float> W15(weight_15,3, 3, 64,64), b15(bias_15,64);
    Buffer<float> W16(weight_16,3, 3, 64,64), b16(bias_16,64);
    Buffer<float> W17(weight_17,3, 3, 64,64), b17(bias_17,64);
    Buffer<float> W18(weight_18,3, 3, 64,64), b18(bias_18,64);
    Buffer<float> W19(weight_19,3, 3, 64,1), b19(bias_19,1);//64 channels mapped into single picture
    //Buffer<float> W20(weight_20,3, 3, 64,64), b20(bias_20,64);
    

    weight[0](n,m,i,o)=W0(n,m,i,o);//W0->weight[0]
    weight[1](n,m,i,o)=W1(n,m,i,o);
    weight[2](n,m,i,o)=W2(n,m,i,o);
    weight[3](n,m,i,o)=W3(n,m,i,o);
    weight[4](n,m,i,o)=W4(n,m,i,o);
    weight[5](n,m,i,o)=W5(n,m,i,o);
    weight[6](n,m,i,o)=W6(n,m,i,o);
    weight[7](n,m,i,o)=W7(n,m,i,o);
    weight[8](n,m,i,o)=W8(n,m,i,o);
    weight[9](n,m,i,o)=W9(n,m,i,o);
    weight[10](n,m,i,o)=W10(n,m,i,o);
    weight[11](n,m,i,o)=W11(n,m,i,o);
    weight[12](n,m,i,o)=W12(n,m,i,o);
    weight[13](n,m,i,o)=W13(n,m,i,o);
    weight[14](n,m,i,o)=W14(n,m,i,o);
    weight[15](n,m,i,o)=W15(n,m,i,o);
    weight[16](n,m,i,o)=W16(n,m,i,o);
    weight[17](n,m,i,o)=W17(n,m,i,o);
    weight[18](n,m,i,o)=W18(n,m,i,o);
    weight[19](n,m,i,o)=W19(n,m,i,o);
    ///weight[20](r.x,r.y,r.z)=W20(r.x,r.y,r.z);    

    b[0](o)=b0(o);
    b[1](o)=b1(o);
    b[2](o)=b2(o);
    b[3](o)=b3(o);
    b[4](o)=b4(o);
    b[5](o)=b5(o);
    b[6](o)=b6(o);
    b[7](o)=b7(o);
    b[8](o)=b8(o);
    b[9](o)=b9(o);
    b[10](o)=b10(o);
    b[11](o)=b11(o);
    b[12](o)=b12(o);
    b[13](o)=b13(o);
    b[14](o)=b14(o);
    b[15](o)=b15(o);
    b[16](o)=b16(o);
    b[17](o)=b17(o);
    b[18](o)=b18(o);
    //b[19](o)=undef<float>();
    b[19](o)=b19(o);
    //b[20](r.w)=b20(r.w);


    
    in_layerd = BoundaryConditions::constant_exterior(in_layer,0,0,in_layer.width(),0,in_layer.height());       
    
    

    in_layerb(n,m,o)=(in_layerd(n,m,o));
    convert2(n,m,o)=cast<float>(0);
    convert2(n,m,0)=16+0.257f*in_layerb(n,m,0)+0.504f*in_layerb(n,m,1)+0.098f*in_layerb(n,m,2);
    convert2(n,m,1)=128+(-0.148f)*in_layerb(n,m,0)+(-0.291f)*in_layerb(n,m,1)+0.439f*in_layerb(n,m,2);
    convert2(n,m,2)=128+(0.439f)*in_layerb(n,m,0)+(-0.368f)*in_layerb(n,m,1)+(-0.071f)*in_layerb(n,m,2);
    convert2(n,m,o)=convert2(n,m,o)/255;//???
 
    
    r={RDom(0, 3, 0, 3, 0, O)};//3*3*o 3*3*64 or 1
    pad=1.5;
    
    ri={RDom(0,3,0,3)};
    in_layert(n,m)=max(16.0f/255,min(235.0f/255,convert2(n,m,0)));
    //f_conv[0].bound(n,0,in_layer.width()).bound(m,0,in_layer.height())
    
    bin = BoundaryConditions::constant_exterior(in_layert,0,0,in_layer.width(),0,in_layer.height());
    
    //most heavy part of the program
    //Target target = get_host_target();
    /*if (target.os == Target::OSX) {
        target.set_feature(Target::Metal);
    } else {
        target.set_feature(Target::OpenCL);
    }*/
    
    f_conv[0]=Func("conv1");//n是宽，m是高
    f_conv[0](n, m, o) = (b[0](o));
    f_conv[0](n, m, o) += weight[0](ri.x, ri.y,0, o) *
            bin(n + ri.x-pad,
                       m + ri.y-pad
                       );
    f_conv[0](n, m, o) = max(0.0f, f_conv[0](n, m, o));
    //Var blockn, threadn, blockm, threadm;
    //f_conv[0].gpu_tile(n, m, blockn, threadn, blockm, threadm, 16, 16);
    //f_conv[0].compile_jit(target);
    //f_conv[0].bound(n,0,in_layer.width()).bound(m,0,in_layer.height());
    

    for (int i=1;i<d-1;i++)
    {
        f_conv[i]=Func("convi");
        bconv[i]=Func("bconv");
    
     //  bconv[i] = BoundaryConditions::constant_exterior(f_conv[i-1], 0);//, 128,
        bconv[i] = BoundaryConditions::constant_exterior(f_conv[i-1],0,0,in_layer.width(),0,in_layer.height());
        f_conv[i](n, m, o) = (b[i](o));
       // f_conv[i].bound(n,0,in_layer.width()).bound(m,0,in_layer.height()).bound(o,0,64);
        f_conv[i](n, m, o) += weight[i](r.x, r.y, r.z, o) *
                bconv[i](n + r.x-pad,
                           m + r.y-pad,
                           r.z);
        f_conv[i](n, m, o) = (max(0.0f, f_conv[i](n, m, o)));
        //f_conv[i].gpu_tile(n, m, blockn, threadn, blockm, threadm, 16, 16);
        //f_conv[i].compile_jit(target);
    }
    
     bconv[d-1] = BoundaryConditions::constant_exterior(f_conv[d-2],0,0,in_layer.width(),0,in_layer.height());
    f_conv[d-1]=Func("convf");
    f_conv[d-1](n, m) = (b[d-1](0));
    f_conv[d-1](n, m) += weight[d-1](r.x, r.y,r.z,0) *
            bconv[d-1](n + r.x-pad,
                       m + r.y-pad,r.z
                       );
    //f_conv[d-1](n, m,c) = max(0, f_conv[d-1](n, m,c));
    //f_conv[d-1].gpu_tile(n, m, blockn, threadn, blockm, threadm, 16, 16);
    //f_conv[d-1].compile_jit(target);
    
    //final(n,m)=((f_conv[d-1](n,m))+in_layert(n,m))*255.0f;
    final(n,m)=((f_conv[d-1](n,m))+in_layert(n,m));
    

    
    
    

    
    newconvert(n,m,o)=convert2(n,m,o);
    newconvert(n,m,0)=final(n,m);
    mconv(n,m,o)=(newconvert(n,m,o))*255;
    convertb(n,m,o)=cast<float>(0);
    convertb(n,m,0)=1.164f*(mconv(n,m,0)-16)+0.00f*(mconv(n,m,1)-128)+1.596f*(mconv(n,m,2)-128);
    convertb(n,m,1)=(1.164f)*(mconv(n,m,0)-16)+(-0.392f)*(mconv(n,m,1)-128)+(-0.813f)*(mconv(n,m,2)-128);
    convertb(n,m,2)=(1.164f)*(mconv(n,m,0)-16)+(2.017f)*(mconv(n,m,1)-128)+(-0.00f)*(mconv(n,m,2)-128);
    output(n,m,o)=cast<uint8_t>(convertb(n,m,o));
    }
  void schedule_for_cpu() {
    Target t=get_target_from_environment();
    t.set_feature(Target::Profile);
    Pipeline p(output);
    output.estimate(n,0,in_layer.width()).estimate(m,0,in_layer.height()).estimate(o,0,3);
    //string st=p.auto_schedule(t);
    //cout<<st<<endl;
    
    for(int i=0;i<d;i++)
    {
    f_conv[i].compute_root();
    if(i<=d-2)  f_conv[i].update().vectorize(n,8).parallel(o);
    
    weight[i].compute_root();
    b[i].compute_root();
    }
    
    convertb.compute_root();
    convert2.compute_root();
    final.compute_root();
    
    output.compile_jit(t);
    //final.print_loop_nest();
    
    Buffer<uint8_t>output_im=output.realize(in_layer.width(),in_layer.height(),3);
    //f_conv[19].realize(128,128,1,t);
    

    //save output image
    convert_and_save_image(output_im,"blh_cpu.png");
    }
  void schedule_for_gpu(){
    Target t=get_target_from_environment();
    if (t.os == Target::OSX) {
        t.set_feature(Target::Metal);
    } else {
        t.set_feature(Target::OpenCL);
    }
    //t.set_feature(Target::CUDA);
    t.set_feature(Target::Profile);
    //Pipeline p(output);
    //output.estimate(n,0,in_layer.width()).estimate(m,0,in_layer.height()).estimate(o,0,3);
    //string st=p.auto_schedule(t);
    //cout<<st<<endl;
    Var blockn, threadn, blockm, threadm, blocko, threado;

    for(int i=0;i<d;i++)
    {
    f_conv[i].compute_root();

    if(i<=d-2)  {
        f_conv[i].reorder(o,n,m).gpu_tile(n ,m, blockn, blockm, threadn, threadm, 4, 4);
        f_conv[i].update(0).reorder(o,n,m).gpu_tile(n ,m, blockn, blockm, threadn, threadm, 4, 4);
        f_conv[i].update(1).reorder(o,n,m).gpu_tile(n ,m, blockn, blockm, threadn, threadm, 4, 4);
    
    }
    
    weight[i].compute_root();
    b[i].compute_root();
    }
    
    convertb.compute_root();
    convert2.compute_root();
    final.compute_root();
    //t.set_feature(Target::CUDA);copy_to_host
    //t.set_feature(Target::Debug);
    //output.compile_jit(t);
    f_conv[3].realize(512,480, 64, t);
    //f_conv[4].compile_to_lowered_stmt("gradient.html", {}, HTML,t);
    //final.print_loop_nest();
    
    //Buffer<uint8_t>output_im=output.realize(in_layer.width(),in_layer.height(),3);
    //f_conv[19].realize(128,128,1,t);
    

    //save output image
    //convert_and_save_image(output_im,"blh_gpu.png");
    //Generating llvm bitcode for function output
    //JIT compiling output
    }
};

int main(int argc, char**argv){

    Buffer<uint8_t> in_layer = load_and_convert_image("blr.png");
  //double t1=current_time();
    vgg p1(in_layer);
    p1.schedule_for_cpu();
  //double t2=current_time();
  //cout<<"CPU time:"<<(t2-t1)/100<<endl;
    //vgg p2(in_layer);
    //p2.schedule_for_gpu();
  //double t3=current_time();
  //cout<<"GPU time:"<<(t3-t2)/100<<endl;
    return 0;
}
