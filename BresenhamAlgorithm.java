package compgrafica;

import image.Image;

public class BresenhamAlgorithm {

    public static void main(String[] args) throws Exception {
        Image image = new Image(400, 400);

        traceRay(200, 200, 250, 399, image);

        image.showImage();
    }

    public static void traceRay(int orgX /*origemx*/, int orgY, int dstX /*destinox*/, int dstY, Image image){
        //tamanho da imagem (w = width / largura, h = height / altura)
        int w = (dstX - orgX), h = (dstY - orgY);

        //vari�veis que v�o ser utilizadas para incrementar os pixels, o d � de "delta".
        //essas vari�veis v�o assumir 0, -1 ou 1, indicando a posi��o no eixo para qual v�o crescer
        //0 = n�o se movimenta, -1 anda pra esquerda ou pra cima, e 1 anda pra direita ou pra baixo (na orienta��o de imagens)
        short dx1 = 0, dy1 = 0, dx2 = 0, dy2 = 0;

        //ajustando as vari�veis para bater com o quadrante escolhido nos par�metros (quadrante 1-cima-direita, 2-cima-esquerda, 3-baixo-esquerda ou 4-baixo-direita)
        if (w < 0){
            dx1 = -1;
            dx2 = -1;
        }else if (w > 0){
            dx1 = 1;
            dx2 = 1;
        }
        if (h < 0){
            dy1 = -1;
        }else if (h > 0){
            dy1 = 1;
        }

        //pegar o maior e menor tamanho (altura ou largura) - imaginando que as imagens de entrada podem ser ret�ngulos
        int longest = Math.abs(w), shortest = Math.abs(h);
        if (longest < shortest){
            //caso esteja invertido, trocar
            longest = Math.abs(h); shortest = Math.abs(w);

            //ajustar quadrante ainda
            dx2 = 0;
            if (h < 0){
                dy2 = -1;
            }else if (h > 0){
                dy2 = 1;
            }
        }

        //vari�vel para alterar o sentido do crescimento (ver descri��o mais abaixo)
        //ela tamb�m usa indiretamente o "ratio" das vari�veis h e w
        int directionShift = longest;

        for (int i=0; i<=longest; i++){
            //desenha ou pinta o pixel de fato na imagem
            image.setPixel(orgX, orgY, 0, 255); //as vari�veis orgX e orgY s�o atualizadas a cada itera��o do for

            //essa vari�vel serve para "inverter o sentido de crescimento dos eixos"
            //assumindo que a reta esteja na diagonal perfeita apontando do centro para o pixel mais da esquerda-baixo...
            //por ex, primeiro o pixel vai andar pra baixo, depois pra direita, depois pra baixo de novo, e assim vai...
            directionShift += shortest;

            //atualizamos a coordenada de origem para a pr�xima itera��o (andamos pixel a pixel)
            if (directionShift > longest){
                directionShift -= longest;
                orgX += dx1; orgY += dy1;
            }else{
                orgX += dx2; orgY += dy2;
            }
        }
    }

}
