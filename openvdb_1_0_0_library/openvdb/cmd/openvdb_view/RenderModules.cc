///////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2012-2013 DreamWorks Animation LLC
//
// All rights reserved. This software is distributed under the
// Mozilla Public License 2.0 ( http://www.mozilla.org/MPL/2.0/ )
//
// Redistributions of source code must retain the above copyright
// and license notice and the following restrictions and disclaimer.
//
// *     Neither the name of DreamWorks Animation nor the names of
// its contributors may be used to endorse or promote products derived
// from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// IN NO EVENT SHALL THE COPYRIGHT HOLDERS' AND CONTRIBUTORS' AGGREGATE
// LIABILITY FOR ALL CLAIMS REGARDLESS OF THEIR BASIS EXCEED US$250.00.
//
///////////////////////////////////////////////////////////////////////////

#include "RenderModules.h"
#include <openvdb/util/logging.h>
#include <math.h>


////////////////////////////////////////

// BufferObject

BufferObject::BufferObject():
    mVertexBuffer(0),
    mNormalBuffer(0),
    mIndexBuffer(0),
    mColorBuffer(0),
    mPrimType(GL_POINTS),
    mPrimNum(0)
{
}

BufferObject::~BufferObject() { clear(); }

void
BufferObject::render() const
{
    if (mPrimNum == 0 || !glIsBuffer(mIndexBuffer) || !glIsBuffer(mVertexBuffer)) {
        OPENVDB_LOG_DEBUG_RUNTIME("request to render empty or uninitialized buffer");
        return;
    }

    bool usesColorBuffer = glIsBuffer(mColorBuffer);
    bool usesNormalBuffer = glIsBuffer(mNormalBuffer);

    // vertex buffer
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
    glVertexAttribPointer(/*VertexAttribArray*/0, /*components*/3,
        /*type*/GL_FLOAT, /*normalized*/GL_FALSE, /*stride*/0, /*firstElementPtr*/0);

    // color buffer
    if (usesColorBuffer) {
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, mColorBuffer);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    }

    // normal buffer
    if (usesNormalBuffer) {
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, mNormalBuffer);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
    }

    // index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
    glDrawElements(mPrimType, mPrimNum, GL_UNSIGNED_INT, 0);

    // disable vertex attribute array.
    glDisableVertexAttribArray(0);
    if (usesColorBuffer) glDisableVertexAttribArray(1);
    if (usesNormalBuffer) glDisableVertexAttribArray(2);

    // release vbo's
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void
BufferObject::genIndexBuffer(const std::vector<GLuint>& v, GLenum primType)
{
    // clear old buffer
    if (glIsBuffer(mIndexBuffer) == GL_TRUE) glDeleteBuffers(1, &mIndexBuffer);

    // gen new buffer
    glGenBuffers(1, &mIndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
    if (glIsBuffer(mIndexBuffer) == GL_FALSE) throw "Error: Unable to create index buffer";

    // upload data
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        sizeof(GLuint) * v.size(), &v[0], GL_STATIC_DRAW); // upload data
    if(GL_NO_ERROR != glGetError()) throw "Error: Unable to upload index buffer data";

    // release buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    mPrimNum = v.size();
    mPrimType = primType;
}

void
BufferObject::genVertexBuffer(const std::vector<GLfloat>& v)
{
    if (glIsBuffer(mVertexBuffer) == GL_TRUE) glDeleteBuffers(1, &mVertexBuffer);

    glGenBuffers(1, &mVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
    if (glIsBuffer(mVertexBuffer) == GL_FALSE) throw "Error: Unable to create vertex buffer";

    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * v.size(), &v[0], GL_STATIC_DRAW);
    if(GL_NO_ERROR != glGetError()) throw "Error: Unable to upload vertex buffer data";

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void
BufferObject::genNormalBuffer(const std::vector<GLfloat>& v)
{
    if (glIsBuffer(mNormalBuffer) == GL_TRUE) glDeleteBuffers(1, &mNormalBuffer);

    glGenBuffers(1, &mNormalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mNormalBuffer);
    if (glIsBuffer(mNormalBuffer) == GL_FALSE) throw "Error: Unable to create normal buffer";

    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * v.size(), &v[0], GL_STATIC_DRAW);
    if(GL_NO_ERROR != glGetError()) throw "Error: Unable to upload normal buffer data";

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void
BufferObject::genColorBuffer(const std::vector<GLfloat>& v)
{
    if (glIsBuffer(mColorBuffer) == GL_TRUE) glDeleteBuffers(1, &mColorBuffer);

    glGenBuffers(1, &mColorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mColorBuffer);
    if (glIsBuffer(mColorBuffer) == GL_FALSE) throw "Error: Unable to create color buffer";

    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * v.size(), &v[0], GL_STATIC_DRAW);
    if(GL_NO_ERROR != glGetError()) throw "Error: Unable to upload color buffer data";

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void
BufferObject::clear()
{
    if (glIsBuffer(mIndexBuffer) == GL_TRUE) glDeleteBuffers(1, &mIndexBuffer);
    if (glIsBuffer(mVertexBuffer) == GL_TRUE) glDeleteBuffers(1, &mVertexBuffer);
    if (glIsBuffer(mColorBuffer) == GL_TRUE) glDeleteBuffers(1, &mColorBuffer);
    if (glIsBuffer(mNormalBuffer) == GL_TRUE) glDeleteBuffers(1, &mNormalBuffer);

    mPrimType = GL_POINTS;
    mPrimNum = 0;
}


////////////////////////////////////////


ShaderProgram::ShaderProgram():
    mProgram(0),
    mVertShader(0),
    mFragShader(0)
{
}

ShaderProgram::~ShaderProgram() { clear(); }

void
ShaderProgram::setVertShader(const std::string& s)
{
    mVertShader = glCreateShader(GL_VERTEX_SHADER);
    if (glIsShader(mVertShader) == GL_FALSE) throw "Error: Unable to create shader program.";

    GLint length = s.length();
    const char *str = s.c_str();
    glShaderSource(mVertShader, 1, &str, &length);

    glCompileShader(mVertShader);
    if(GL_NO_ERROR != glGetError()) throw "Error: Unable to compile vertex shader.";
}

void
ShaderProgram::setFragShader(const std::string& s)
{
    mFragShader = glCreateShader(GL_FRAGMENT_SHADER);
    if (glIsShader(mFragShader) == GL_FALSE) throw "Error: Unable to create shader program.";

    GLint length = s.length();
    const char *str = s.c_str();
    glShaderSource(mFragShader, 1, &str, &length);

    glCompileShader(mFragShader);
    if(GL_NO_ERROR != glGetError()) throw "Error: Unable to compile fragment shader.";
}

void
ShaderProgram::build()
{
    mProgram = glCreateProgram();
    if (glIsProgram(mProgram) == GL_FALSE) throw "Error: Unable to create shader program.";

    if(glIsShader(mVertShader) == GL_TRUE) glAttachShader(mProgram, mVertShader);
    if(GL_NO_ERROR != glGetError()) throw "Error: Unable to attach vertex shader.";

    if(glIsShader(mFragShader) == GL_TRUE) glAttachShader(mProgram, mFragShader);
    if(GL_NO_ERROR != glGetError()) throw "Error: Unable to attach fragment shader.";


    glLinkProgram(mProgram);

    GLint linked;
    glGetProgramiv(mProgram, GL_LINK_STATUS, &linked);

    if (!linked) throw "Error: Unable to link shader program.";
}

void
ShaderProgram::build(const std::vector<GLchar*>& attributes)
{
    mProgram = glCreateProgram();
    if (glIsProgram(mProgram) == GL_FALSE) throw "Error: Unable to create shader program.";


    for(GLuint n = 0, N = attributes.size(); n < N; ++n) {
        glBindAttribLocation(mProgram, n, attributes[n]);
    }

    if(glIsShader(mVertShader) == GL_TRUE) glAttachShader(mProgram, mVertShader);
    if(GL_NO_ERROR != glGetError()) throw "Error: Unable to attach vertex shader.";

    if(glIsShader(mFragShader) == GL_TRUE) glAttachShader(mProgram, mFragShader);
    if(GL_NO_ERROR != glGetError()) throw "Error: Unable to attach fragment shader.";


    glLinkProgram(mProgram);

    GLint linked;
    glGetProgramiv(mProgram, GL_LINK_STATUS, &linked);

    if (!linked) throw "Error: Unable to link shader program.";
}

void
ShaderProgram::startShading() const
{
    if(glIsProgram(mProgram) == GL_FALSE)
        throw "Error: called startShading() on uncompiled shader program.";

    glUseProgram(mProgram);
}

void
ShaderProgram::stopShading() const
{
    glUseProgram(0);
}

void
ShaderProgram::clear()
{
    GLsizei numShaders;
    GLuint shaders[2];

    glGetAttachedShaders(mProgram, 2, &numShaders, shaders);

    // detach and remove shaders
    for (GLsizei n = 0; n < numShaders; ++numShaders) {

        glDetachShader(mProgram, shaders[n]);

        if(glIsShader(shaders[n]) == GL_TRUE) glDeleteShader(shaders[n]);
    }

    // remove program
    if(glIsProgram(mProgram)) glDeleteProgram(mProgram);
}


////////////////////////////////////////


openvdb::Vec3s TreeTopologyOp::sNodeColors[] = {
    openvdb::Vec3s(0.045, 0.045, 0.045),            // root
    openvdb::Vec3s(0.0432, 0.33, 0.0411023),        // first internal node level
    openvdb::Vec3s(0.871, 0.394, 0.01916),          // intermediate internal node levels
    openvdb::Vec3s(0.00608299, 0.279541, 0.625)     // leaf nodes
};


////////////////////////////////////////

// ViewPortModule

ViewPortModule::ViewPortModule():
    mAxisGnomeScale(1.5),
    mGroundPlaneScale(8.0)
{
}

void
ViewPortModule::render()
{
    /// @todo use VBO's

    // Ground plane
    glPushMatrix();
    glScalef(mGroundPlaneScale, mGroundPlaneScale, mGroundPlaneScale);
    glColor3f( 0.6, 0.6, 0.6);

    OPENVDB_NO_FP_EQUALITY_WARNING_BEGIN

    float step = 0.125;
    for (float x = -1; x < 1.125; x+=step) {

        if (fabs(x) == 0.5 || fabs(x) == 0.0) {
            glLineWidth(1.5);
        } else {
            glLineWidth(1.0);
        }

        glBegin( GL_LINES );
        glVertex3f(x, 0, 1);
        glVertex3f(x, 0, -1);
        glVertex3f(1, 0, x);
        glVertex3f(-1, 0, x);
        glEnd();
    }

    OPENVDB_NO_FP_EQUALITY_WARNING_END


    glPopMatrix();

    // Axis gnome
    GLfloat modelview[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, &modelview[0]);

    // Stash current viewport settigs.
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, &viewport[0]);

    GLint width = viewport[2] / 20;
    GLint height = viewport[3] / 20;
    glViewport(0, 0, width, height);


    glPushMatrix();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


    double campos[3] = {0.0 + modelview[2], 0.0 + modelview[6], 0.0 + modelview[10]};
    double up[3] = {modelview[1], modelview[5], modelview[9]};

    gluLookAt(campos[0], campos[1], campos[2], 0.0, 0.0, 0.0, up[0], up[1], up[2]);

    glScalef(mAxisGnomeScale, mAxisGnomeScale, mAxisGnomeScale);

    glLineWidth(2.0);

    glBegin(GL_LINES);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0, 0, 0);
    glVertex3f(1, 0, 0);

    glColor3f(0.0f, 1.0f, 0.0f );
    glVertex3f(0, 0, 0);
    glVertex3f(0, 1, 0);

    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 1);
    glEnd();

    glLineWidth(1.0);

    // reset viewport
    glPopMatrix();
    glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
}


////////////////////////////////////////

// Tree topology render module

TreeTopologyModule::TreeTopologyModule(const std::vector<openvdb::GridBase::ConstPtr>& grids):
    mGrids(&grids),
    mBufferObjects(grids.size()),
    mIsInitialized(false)
{
    mShader.setVertShader(
        "#version 130\n"
        "in vec3 vertex;\n"
        "in vec3 color;\n"
        "out vec4 f_color;\n"
        "void main() {\n"
        "f_color = vec4(color, 1.0);\n"
        "gl_Position =  gl_ModelViewProjectionMatrix * vec4(vertex, 1.0);}\n");

    mShader.setFragShader(
        "#version 130\n"
        "in vec4 f_color;\n"
        "out vec4 out_Color;\n"
        "void main() {\n"
            "out_Color = f_color;}\n");

    mShader.build();
}

void
TreeTopologyModule::init()
{
    mIsInitialized = true;

    // extract grid topology
    for (size_t n = 0, N = mGrids->size(); n < N; ++n) {

        openvdb::GridBase::ConstPtr grid = (*mGrids)[n];

        TreeTopologyOp drawTopology(mBufferObjects[n]);

        if (!util::processTypedGrid(grid, drawTopology)) {
            OPENVDB_LOG_INFO("Ignoring unrecognized grid type"
                " during tree topology module initialization.");
        }
    }
}

void
TreeTopologyModule::render()
{
    if (!mIsInitialized) init();

    mShader.startShading();

    for (size_t n = 0, N = mBufferObjects.size(); n < N; ++n) {
        mBufferObjects[n].render();
    }

    mShader.stopShading();
}


////////////////////////////////////////

// Meshing module

MeshModule::MeshModule(const std::vector<openvdb::GridBase::ConstPtr>& grids):
    mGrids(&grids),
    mBufferObjects(grids.size()),
    mIsInitialized(false)
{
    mShader.setVertShader(
        "#version 130\n"
        "in vec3 vertex;\n"
        "in vec3 v_normal;\n"
        "out vec3 normal;\n"
        "out vec3 light_dir;\n"
        "void main() {\n"
            "normal = normalize(gl_NormalMatrix * gl_Normal);\n"
            "vec3 mv_vert = vec3(gl_ModelViewMatrix * vec4(vertex, 1.0));"
            "light_dir = vec3(0.0, 0.0, -1.0) - mv_vert;"
            "gl_Position =  gl_ModelViewProjectionMatrix * vec4(vertex, 1.0);\n"
        "}\n");

    mShader.setFragShader(
        "#version 130\n"
        "in vec3 normal;\n"
        "in vec3 light_dir;\n"
        "out vec4 out_Color;\n"
        "void main() {\n"
            "const vec4 diffuseColor = vec4(0.6, 0.6, 0.6, 1.0);\n"
            "vec3 normalized_normal = normalize(normal);\n"
            "vec3 normalized_light_dir = normalize(light_dir);\n"
            "float diffuseTerm = clamp(dot(normalized_normal, normalized_light_dir), 0.2, 1.0);\n"
            "out_Color = vec4(0.2, 0.2, 0.2, 1.0) + diffuseTerm * diffuseColor;\n"
        "}\n");

    mShader.build();
}

void
MeshModule::init()
{
    mIsInitialized = true;

    // extract grid topology
    for (size_t n = 0, N = mGrids->size(); n < N; ++n) {

        openvdb::GridBase::ConstPtr grid = (*mGrids)[n];

        MeshOp drawMesh(mBufferObjects[n]);

        if (!util::processTypedScalarGrid(grid, drawMesh)) {
            OPENVDB_LOG_INFO(
                "Ignoring non-scalar grid type during mesh module initialization.");
        }
    }
}


void
MeshModule::render()
{


    if (!mIsInitialized) init();

    mShader.startShading();

    for (size_t n = 0, N = mBufferObjects.size(); n < N; ++n) {
        mBufferObjects[n].render();
    }

    mShader.stopShading();
}

// Copyright (c) 2012-2013 DreamWorks Animation LLC
// All rights reserved. This software is distributed under the
// Mozilla Public License 2.0 ( http://www.mozilla.org/MPL/2.0/ )
