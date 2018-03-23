#include <stdio.h>

//codigo com falha de segmentação, será corrigido mais tarde
typedef struct _pixel {
  unsigned short int vermelho;
  unsigned short int verde;
  unsigned short int azul;
} Pixel;

typedef struct _imagem {
  // pixel[largura][altura]
  Pixel pixel[512][512];
  unsigned int largura;
  unsigned int altura;
} Imagem;

void lerCabecalhoImagem(Imagem imagem) {
  int cor;
  char tipo[4];
  scanf("%s", tipo);
  scanf("%u %u %d", &imagem.largura, &imagem.altura, &cor);
}

void lerCabecalhoPixel(Imagem imagem) {
  for (unsigned int i = 0; i < imagem.altura; ++i) {
    for (unsigned int j = 0; j < imagem.largura; ++j) {
      scanf("%hu %hu %hu", &imagem.pixel[i][j].vermelho,
                           &imagem.pixel[i][j].verde,
                           &imagem.pixel[i][j].azul);
    }
  }
}

void lerImagem(Imagem imagem) {
  lerCabecalhoImagem(imagem);
  lerCabecalhoPixel(imagem);
}

Imagem escalaDeCinza(Imagem imagem) {
  for (unsigned int i = 0; i < imagem.altura; ++i) {
    for (unsigned int j = 0; j < imagem.largura; ++j) {
      int media = imagem.pixel[i][j].vermelho +
                  imagem.pixel[i][j].verde +
                  imagem.pixel[i][j].azul;
      media /= 3;
      imagem.pixel[i][j].vermelho = media;
      imagem.pixel[i][j].verde = media;
      imagem.pixel[i][j].azul = media;
    }
  }
  return imagem;
}

int checarTamanhoPixel(int numPixel) {
  int tamanhoPixel = 0;
  int maxRGB = 255;

  if(numPixel < maxRGB) {
    tamanhoPixel = numPixel;
  } else {
    tamanhoPixel = maxRGB;
  }
  return tamanhoPixel;
}

Imagem sepia(Imagem imagem) {
  for (unsigned int x = 0; x < imagem.altura; ++x) {
    for (unsigned int y = 0; y < imagem.largura; ++y) {
      unsigned short int pixel[3];
      pixel[0] = imagem.pixel[x][y].vermelho;
      pixel[1] = imagem.pixel[x][y].verde;
      pixel[2] = imagem.pixel[x][y].azul;

      int numPixel;
      numPixel =  pixel[0] * .393 + pixel[1] * .769 + pixel[2] * .189;
      imagem.pixel[x][y].vermelho = checarTamanhoPixel(numPixel);

      numPixel =  pixel[0] * .349 + pixel[1] * .686 + pixel[2] * .168;
      imagem.pixel[x][y].vermelho = checarTamanhoPixel(numPixel);

      numPixel =  pixel[0] * .272 + pixel[1] * .534 + pixel[2] * .131;
      imagem.pixel[x][y].vermelho = checarTamanhoPixel(numPixel);
    }
  }
  return imagem;
}

int min(int a, int b) {
  if (a < b) {
    return a;
  } else {
  return b;
  }
}

int max(int a, int b) {
  if (a > b) {
    return a;
  } else {
  return b;
  }
}

Imagem blur(Imagem imagem) {
  int tamanho = 0;
  scanf("%d", &tamanho);

  for (unsigned int i = 0; i < imagem.altura; ++i) {
    for (unsigned int j = 0; j < imagem.largura; ++j) {
      int nulo = 0;
      Pixel media = {0, 0, 0};
      int minAltura = min(imagem.altura - 1, i + tamanho/2);
      int minLargura = min(imagem.largura - 1, j + tamanho/2);
      int x = max(nulo, i - tamanho/2);
      for(x; x <= minAltura; ++x) {
        int y = max(nulo, j - tamanho/2);
        for(y; y <= minLargura; ++y) {
          media.vermelho += imagem.pixel[x][y].vermelho;
          media.verde += imagem.pixel[x][y].verde;
          media.azul += imagem.pixel[x][y].azul;
        }
      }
      media.vermelho /= tamanho * tamanho;
      media.verde /= tamanho * tamanho;
      media.azul /= tamanho * tamanho;

      imagem.pixel[i][j].vermelho = media.vermelho;
      imagem.pixel[i][j].verde = media.verde;
      imagem.pixel[i][j].azul = media.azul;
    }
  }
  return imagem;
}

Imagem rotacionar90direita(Imagem imagem) {
  Imagem rotacionada;
  rotacionada.largura = imagem.largura;
  rotacionada.altura = imagem.altura;

  for (unsigned int i = 0, y = 0; i < rotacionada.altura; ++i, ++y) {
    for (int j = rotacionada.largura - 1, x = 0; j >= 0; --j, ++x) {
      rotacionada.pixel[i][j].vermelho = imagem.pixel[x][y].vermelho;
      rotacionada.pixel[i][j].verde = imagem.pixel[x][y].verde;
      rotacionada.pixel[i][j].azul = imagem.pixel[x][y].azul;
    }
  }
  return rotacionada;
}

Imagem rotacionar(Imagem imagem) {
  int quantasVezes = 0;
  scanf("%d", &quantasVezes);
  quantasVezes %= 4;
  for (int j = 0; j < quantasVezes; ++j) {
    imagem = rotacionar90direita(imagem);
  }
  return imagem;
}

Imagem espelhar(Imagem imagem) {
  int horizontal = 0;
  scanf("%d", &horizontal);
  int largura = imagem.largura, altura = imagem.altura;

  if (horizontal == 1) {
    largura /= 2;
  } else {
    altura /= 2;
  }
  for (int i = 0; i < altura; ++i) {
    for (int j = 0; j < largura; ++j) {
      int x = i, y = j;
      if (horizontal == 1) {
        y = imagem.largura - 1 - j;
      } else {
        x = imagem.altura - 1 - i;
      }
      Pixel aux1;
      aux1.vermelho = imagem.pixel[i][j].vermelho;
      aux1.verde = imagem.pixel[i][j].verde;
      aux1.azul = imagem.pixel[i][j].azul;

      imagem.pixel[i][j].vermelho = imagem.pixel[x][y].vermelho;
      imagem.pixel[i][j].verde = imagem.pixel[x][y].verde;
      imagem.pixel[i][j].azul = imagem.pixel[x][y].azul;

      imagem.pixel[x][y].vermelho = aux1.vermelho;
      imagem.pixel[x][y].verde = aux1.verde;
      imagem.pixel[x][y].azul = aux1.azul;
    }
  }
  return imagem;
}

Imagem inverterCores(Imagem imagem) {
  for (unsigned int i = 0; i < imagem.altura; ++i) {
    for (unsigned int j = 0; j < imagem.largura; ++j) {
      imagem.pixel[i][j].vermelho = 255 - imagem.pixel[i][j].vermelho;
      imagem.pixel[i][j].verde = 255 - imagem.pixel[i][j].verde;
      imagem.pixel[i][j].azul = 255 - imagem.pixel[i][j].azul;
    }
  }
  return imagem;
}

int pixelEigual(Pixel pixel1, Pixel pixel2) {
  if (pixel1.vermelho == pixel2.vermelho &&
    pixel1.verde == pixel2.verde &&
    pixel1.azul == pixel2.azul)
    return 1;
  return 0;
}

Imagem cortarImagem(Imagem imagem ) {
  Imagem cortada;
  int x, y;
  int largura, altura;
  scanf("%d %d", &x, &y);
  scanf("%d %d", &largura, &altura);
  cortada.largura = largura;
  cortada.altura = altura;

  for(int i = 0; i < altura; ++i) {
    for(int j = 0; j < largura; ++j) {
      cortada.pixel[i][j].vermelho = imagem.pixel[i + y][j + x].vermelho;
      cortada.pixel[i][j].verde = imagem.pixel[i + y][j + x].verde;
      cortada.pixel[i][j].azul = imagem.pixel[i + y][j + x].azul;
    }
  }
  return cortada;
}

void printarImagem(Imagem imagem){
  printf("P3\n");
  printf("%u %u\n255\n", imagem.largura, imagem.altura);

  for (unsigned int i = 0; i < imagem.altura; ++i) {
      for (unsigned int j = 0; j < imagem.largura; ++j) {
          printf("%hu %hu %hu ", imagem.pixel[i][j].vermelho,
                                 imagem.pixel[i][j].verde,
                                 imagem.pixel[i][j].azul);
      }
      printf("\n");
  }
}

int main() {
  Imagem imagem;
  lerImagem(imagem);

  int nOpcoes;
  scanf("%d", &nOpcoes);

  for(int i = 0; i < nOpcoes; ++i) {
    int opcao;
    scanf("%d", &opcao);

    switch(opcao) {
      case 1: {
        imagem = escalaDeCinza(imagem);
        break;
      }
      case 2: {
        imagem = sepia(imagem);
        break;
      }
      case 3: {
        imagem = blur(imagem);
        break;
      }
      case 4: {
        imagem = rotacionar(imagem);
        break;
      }
      case 5: {
        imagem = espelhar(imagem);
        break;
      }
      case 6: {
        imagem = inverterCores(imagem);
        break;
      }
      case 7: {
        imagem = cortarImagem(imagem);
        break;
      }
    }
  }
  printarImagem(imagem);
  return 0;
}
