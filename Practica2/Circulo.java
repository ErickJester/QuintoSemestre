//*****************************************************************************************
//CIRCULO.JAVA
//*****************************************************************************************
//*****************************************************************************************
//HERNANDEZ HERNANDEZ RUBEN
//Curso: Compiladores 3CM15
//MAYO 2021
//ESCOM-IPN
//YACC EN JAVA GRAFIBASI
//*****************************************************************************************
import java.awt.*;

public class Circulo implements Dibujable {
	private int x=0;
	private int y=0;
	private int r=0;

	public Circulo(int x, int y, int r)
	{
		this.x=x;
		this.y=y;
		this.r=r;
	}

	public void dibuja(Graphics g){
		g.drawOval(x,y,r,r);
	}
}


