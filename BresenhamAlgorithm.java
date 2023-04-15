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

        //variáveis que vão ser utilizadas para incrementar os pixels, o d é de "delta".
        //essas variáveis vão assumir 0, -1 ou 1, indicando a posição no eixo para qual vão crescer
        //0 = não se movimenta, -1 anda pra esquerda ou pra cima, e 1 anda pra direita ou pra baixo (na orientação de imagens)
        short dx1 = 0, dy1 = 0, dx2 = 0, dy2 = 0;

        //ajustando as variáveis para bater com o quadrante escolhido nos parâmetros (quadrante 1-cima-direita, 2-cima-esquerda, 3-baixo-esquerda ou 4-baixo-direita)
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

        //pegar o maior e menor tamanho (altura ou largura) - imaginando que as imagens de entrada podem ser retângulos
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

        //variável para alterar o sentido do crescimento (ver descrição mais abaixo)
        //ela também usa indiretamente o "ratio" das variáveis h e w
        int directionShift = longest;

        for (int i=0; i<=longest; i++){
            //desenha ou pinta o pixel de fato na imagem
            image.setPixel(orgX, orgY, 0, 255); //as variáveis orgX e orgY são atualizadas a cada iteração do for

            //essa variável serve para "inverter o sentido de crescimento dos eixos"
            //assumindo que a reta esteja na diagonal perfeita apontando do centro para o pixel mais da esquerda-baixo...
            //por ex, primeiro o pixel vai andar pra baixo, depois pra direita, depois pra baixo de novo, e assim vai...
            directionShift += shortest;

            //atualizamos a coordenada de origem para a próxima iteração (andamos pixel a pixel)
            if (directionShift > longest){
                directionShift -= longest;
                orgX += dx1; orgY += dy1;
            }else{
                orgX += dx2; orgY += dy2;
            }
        }
    }

}
