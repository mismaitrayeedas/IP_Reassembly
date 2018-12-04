/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package networking;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;


/**
 *
 * @author MAITRAIEE
 */
public class Networking {
    class datagram {
                String ip;
		int id;
		datagram link;
		fragment child;
    }
    class fragment {
                int fragment_offset;
		int more_fragment;
                int length;
		fragment clink;
    }
    
    datagram start;
    
    public void insert_datagram(String ip,int id,int frag_off,int more_frag,int length){
        Networking obj1=new Networking();
        datagram head=new datagram();
        datagram temp=start;
        head.ip=ip;
        head.id=id;
        head.link=null;
        head.child=null;
        //System.out.println(head.ip);
        //System.out.println(head.id);

        
        if(start==null){
            start=head;
            obj1.insert_fragment(frag_off,more_frag,length,head);
        }
        else{
            if(temp.link==null){
                
                if(temp.ip.equals(head.ip) && temp.id==head.id){
                   // System.out.println("ip matched");
                   obj1. insert_fragment(frag_off,more_frag,length,temp);
                }
                else{
                    temp.link=head;
                    obj1.insert_fragment(frag_off,more_frag,length,head);
                }
            }
            else{
                while(temp.link!=null){
                     if(temp.ip.equals(head.ip) && temp.id==head.id){
                        //  System.out.println("ip matched");
                         break;
                     }
                    temp=temp.link;
                }
                if(temp.ip.equals(head.ip) && temp.id==head.id){
                    // System.out.println("ip matched");
                     obj1.insert_fragment(frag_off,more_frag,length,temp);
                }
                else{
                    temp.link=head;
                    temp=temp.link;
                    obj1.insert_fragment(frag_off,more_frag,length,temp);       
                }   
            }
            
        }
        
    }
    
    public void insert_fragment(int frag_off,int more_frag,int length,datagram parent){
        fragment head=new fragment();
        fragment temp=parent.child;
        head.fragment_offset=frag_off;
        head.more_fragment=more_frag;
        head.length=length;
        head.clink=null;
        
        //System.out.println(parent.ip);
        //System.out.println(+parent.id);
        
        if(temp == null){
            parent.child=head;
            //System.out.println(+head.fragment_offset);
            //System.out.println(+head.length);
        }
        else{
            if(temp.clink==null){
                if(temp.fragment_offset==head.fragment_offset && temp.length==head.length && temp.more_fragment==head.more_fragment){
                    System.out.println(parent.ip+"\t"+parent.id+"\tDuplicate fragment");
                    
                }
                else{
                    temp.clink=head;
                    //System.out.println(+head.fragment_offset);
                    //System.out.println(+head.length);
                            
                }
            }
            else{
               int i=0;
                while(temp.clink != null){
                   if(temp.fragment_offset==head.fragment_offset && temp.length==head.length && temp.more_fragment==head.more_fragment){
                    System.out.println(parent.ip+"\t"+parent.id+"\tDuplicate fragment");
                    i=1;
                    break;
                    } 
                  temp=temp.clink;   
                }
                if(i==0){
                    temp.clink=head;
                    //System.out.println(+head.fragment_offset);
                    //System.out.println(+head.length);
                }
            }
        }
        //System.out.println("child id created successfuly");
        
    }
    
     public void display(){
       datagram temp=start;
       int current_length,total_length,check,i=0;
       while(temp != null){
           
          fragment child=new fragment();
          current_length=0;
          total_length=0;
          check=0;
          child=temp.child;
          
         while(child !=null){
              if(child.more_fragment==0){
                  total_length=child.fragment_offset+child.length-20;
              }
              if((child.fragment_offset==0)&&(child.more_fragment==0)){
                  System.out.println(temp.ip+"\t"+temp.id+"\t"+"Incomplite Inconsist data");
                  child=null;
                  check=check+1;
                  //break;
              }
              else{
                  current_length=current_length+child.length-20;
                  child=child.clink;
              }
         }
         if(check==0){
             if(current_length==total_length){
                 System.out.println(temp.ip+"\t"+temp.id+"\t"+"Complete fragment");
             }
             else if(current_length>total_length){
                 System.out.println(temp.ip+"\t"+temp.id+"\t"+"Length Mismatch");
             }
             else if(current_length<total_length){
                 System.out.println(temp.ip+"\t"+temp.id+"\t"+"Fragment missing");
             }
             
         }
         temp=temp.link;
       }
       
    }
    
    public static void main(String[] args) throws IOException{
        Networking obj=new Networking();
        
        
        BufferedReader reader = new BufferedReader(new FileReader("ip.txt"));
            String line=reader.readLine();
            
            while(line != null){
                String info[]=line.split(" ",10);
                String ip=info[0];
                int id=Integer.parseInt(info[2]);
                int frag_off=Integer.parseInt(info[4]);
                int more_frag=Integer.parseInt(info[6]);
                int length=Integer.parseInt(info[8]);
                
                obj.insert_datagram(ip,id,frag_off,more_frag,length);
                line=reader.readLine();
            }
            obj.display();
           reader.close();
        }
        
   
    }
    

