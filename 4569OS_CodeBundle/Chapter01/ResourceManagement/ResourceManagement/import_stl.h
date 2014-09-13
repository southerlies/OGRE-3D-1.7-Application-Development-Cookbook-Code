#ifndef __VCGLIB_IMPORT_STL
#define __VCGLIB_IMPORT_STL
#include &lt;stdio.h&gt;
#include &lt;wrap/callback.h&gt;
#include &lt;vcg/complex/trimesh/allocate.h&gt;

namespace vcg {
namespace tri {
namespace io {

/** 
This class encapsulate a filter for importing stl (stereolitograpy) meshes.
The stl format is quite simple and rather un-flexible. It just stores, in ascii or binary the, unindexed, geometry of the faces.
Warning: this code assume little endian (PC) architecture!!!
*/
template &lt;class OpenMeshType&gt;
class ImporterSTL
{
public:

typedef typename OpenMeshType::VertexPointer VertexPointer;
typedef typename OpenMeshType::ScalarType ScalarType;
typedef typename OpenMeshType::VertexType VertexType;
typedef typename OpenMeshType::FaceType FaceType;
typedef typename OpenMeshType::VertexIterator VertexIterator;
typedef typename OpenMeshType::FaceIterator FaceIterator;

// if it is binary there are 80 char of comment, the number fn of faces and then exactly fn*4*3 bytes.

enum {STL_LABEL_SIZE=80};

class STLFacet
{
public:
  Point3f n;
  Point3f v[3];
//  short attr;
};

enum STLError {
	E_NOERROR,				// 0
		// Errori di open
	E_CANTOPEN,				// 1
	E_UNESPECTEDEOF       		        // 2
};

static const char *ErrorMsg(int error)
{
  static const char * stl_error_msg[] =
  {
	"No errors",
	"Can't open file",
	"Premature End of file",
	};

  if(error&gt;2 || error&lt;0) return "Unknown error";
  else return stl_error_msg[error];
};

static int Open(OpenMeshType &amp;mesh, const char *filename, int &amp;loadmask, CallBackPos *cb=0)
{
  loadmask = Mask::IOM_VERTCOORD | Mask::IOM_FACEINDEX;
  return Open(mesh,filename,cb);
}

static int Open( OpenMeshType &amp;m, const char * filename, CallBackPos *cb=0)
{
  FILE *fp;
  bool binary=false;
  fp = fopen(filename, "r");
  if(fp == NULL)
    {
      return E_CANTOPEN;
    }

  /* Find size of file */
  fseek(fp, 0, SEEK_END);
  int file_size = ftell(fp);
  int facenum;
  /* Check for binary or ASCII file */
  fseek(fp, STL_LABEL_SIZE, SEEK_SET);
  fread(&amp;facenum, sizeof(int), 1, fp);
  int expected_file_size=STL_LABEL_SIZE + 4 + (sizeof(short)+sizeof(STLFacet) )*facenum ;
  if(file_size ==  expected_file_size) binary = true;
  unsigned char tmpbuf[128];
  fread(tmpbuf,sizeof(tmpbuf),1,fp);
  for(unsigned int i = 0; i &lt; sizeof(tmpbuf); i++)
    {
      if(tmpbuf[i] &gt; 127)
 	      {
	        binary=true;
	        break;
	      }
    }
  // Now we know if the stl file is ascii or binary.
  fclose(fp);
  if(binary) return OpenBinary(m,filename,cb);
  else return OpenAscii(m,filename,cb);
}

static int OpenBinary( OpenMeshType &amp;m, const char * filename, CallBackPos *cb=0)
{
  FILE *fp;
  fp = fopen(filename, "rb");
  if(fp == NULL)
  {
    return E_CANTOPEN;
  }
   
  int facenum;
  fseek(fp, STL_LABEL_SIZE, SEEK_SET);
  fread(&amp;facenum, sizeof(int), 1, fp);
  
  m.Clear();
  FaceIterator fi=Allocator&lt;OpenMeshType&gt;::AddFaces(m,facenum);
  VertexIterator vi=Allocator&lt;OpenMeshType&gt;::AddVertices(m,facenum*3);
  // For each triangle read the normal, the three coords and a short set to zero
	for(int i=0;i&lt;facenum;++i)
    {
      short attr;
      Point3f norm;
      Point3f tri[3];
      fread(&amp;norm,sizeof(Point3f),1,fp);
      fread(&amp;tri,sizeof(Point3f),3,fp);
      fread(&amp;attr,sizeof(short),1,fp);
      for(int k=0;k&lt;3;++k)
      {
        (*vi).P().Import(tri[k]); 
        (*fi).V(k)=&amp;*vi; 
        ++vi;
      }
      ++fi;
      if(cb &amp;&amp; (i%1000)==0) cb((i*100)/facenum,"STL Mesh Loading");	
    }
    fclose(fp);
    return E_NOERROR;
  }


  static int OpenAscii( OpenMeshType &amp;m, const char * filename, CallBackPos *cb=0)
  {
    FILE *fp;
    fp = fopen(filename, "r");
    if(fp == NULL)
    {
      return E_CANTOPEN;
    }
		long currentPos = ftell(fp);
		fseek(fp,0L,SEEK_END);
		long fileLen = ftell(fp);
		fseek(fp,currentPos,SEEK_SET);

    m.Clear();
  
    /* Skip the first line of the file */
    while(getc(fp) != '\n') { }

    STLFacet f;
    int cnt=0;
		int lineCnt=0;
		int ret;
    /* Read a single facet from an ASCII .STL file */
    while(!feof(fp))
    {
      if(cb &amp;&amp; (++cnt)%1000)   cb( int(double(ftell(fp))*100.0/fileLen), "STL Mesh Loading");	
	    ret=fscanf(fp, "%*s %*s %f %f %f\n", &amp;f.n.X(), &amp;f.n.Y(), &amp;f.n.Z()); // --&gt; "facet normal 0 0 0"
			if(ret!=3) 
			{
				// we could be in the case of a multiple solid object, where after a endfaced instead of another facet we have to skip two lines:
				//     endloop
				//	 endfacet
				//endsolid     &lt;- continue on ret==0 will skip this line
				//solid ascii  &lt;- and this one.
				//   facet normal 0.000000e+000 7.700727e-001 -6.379562e-001
				lineCnt++;
				continue; 
			}
      ret=fscanf(fp, "%*s %*s"); // --&gt; "outer loop"
      ret=fscanf(fp, "%*s %f %f %f\n", &amp;f.v[0].X(),  &amp;f.v[0].Y(),  &amp;f.v[0].Z()); // --&gt; "vertex x y z"
			if(ret!=3) 
				return E_UNESPECTEDEOF;
      ret=fscanf(fp, "%*s %f %f %f\n", &amp;f.v[1].X(),  &amp;f.v[1].Y(),  &amp;f.v[1].Z()); // --&gt; "vertex x y z"
			if(ret!=3) 
				return E_UNESPECTEDEOF;
      ret=fscanf(fp, "%*s %f %f %f\n", &amp;f.v[2].X(),  &amp;f.v[2].Y(),  &amp;f.v[2].Z()); // --&gt; "vertex x y z"
			if(ret!=3) 
				return E_UNESPECTEDEOF;
      ret=fscanf(fp, "%*s"); // --&gt; "endloop"
      ret=fscanf(fp, "%*s"); // --&gt; "endfacet"
			lineCnt+=7;
      if(feof(fp)) break;
      FaceIterator fi=Allocator&lt;OpenMeshType&gt;::AddFaces(m,1);
      VertexIterator vi=Allocator&lt;OpenMeshType&gt;::AddVertices(m,3);
      for(int k=0;k&lt;3;++k)
      {
        (*vi).P().Import(f.v[k]); 
        (*fi).V(k)=&amp;*vi; 
        ++vi;
      }    
    }
    fclose(fp);
    return E_NOERROR;
  }
}; // end class
} // end Namespace tri
} // end Namespace io
} // end Namespace vcg

#endif
</PRE></BODY></HTML>
