
void DrawTest()
{
    
}

void DrawQuad()
{
    glShadeModel(GL_SMOOTH);
    glBegin(GL_TRIANGLES);
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f( 0.1, 0.1, 0.1);  
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f( 0.1, -0.1, 0.1);
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f( 0.1, -0.1, -0.1);
    glEnd();
    
    glShadeModel(GL_SMOOTH);
    glBegin(GL_TRIANGLES);
    glVertex3f( 0.1, 0.1, 0.1);  
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f( 0.1, -0.1, -0.1);
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f( 0.1, 0.1, -0.1);
    glColor3f(0.0, 0.0, 1.0);
    glEnd();
    
    glBegin(GL_TRIANGLES);
    glVertex3f( -0.1, 0.1, 0.1);  
    glVertex3f( -0.1, -0.1, -0.1);
    glVertex3f( -0.1, 0.1, -0.1);
    glEnd();
    
    
    glBegin(GL_TRIANGLES);
    glVertex3f( -0.1, 0.1, 0.1);  
    glVertex3f( -0.1, -0.1, -0.1);
    glVertex3f( -0.1, -0.1, 0.1);
    glEnd();
    
    
    glBegin(GL_TRIANGLES);
    glVertex3f( 0.1, 0.1, 0.1);  
    glVertex3f( -0.1, 0.1, 0.1);
    glVertex3f( 0.1, 0.1, -0.1);
    glEnd();
    
    
    glBegin(GL_TRIANGLES);
    glVertex3f( 0.1, -0.1, 0.1);  
    glVertex3f( -0.1, -0.1, 0.1);
    glVertex3f( -0.1, -0.1, -0.1);
    glEnd();
    
    
}

void BicubicSurface(void)
{
    
}