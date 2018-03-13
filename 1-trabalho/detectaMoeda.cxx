#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkBinaryThresholdImageFilter.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkImageFileWriter.h"
#include "itkBinaryMorphologicalClosingImageFilter.h"
#include "itkBinaryBallStructuringElement.h"

 
int main(int argc, char *argv[]) {

	if(argc < 2) { 
		std::cerr << "Usage: " << std::endl;
		std::cerr << argv[0] << " inputImageFile" << std::endl;
		return EXIT_FAILURE;
	}

	unsigned int MAX_COL, MAX_LIN;
	unsigned int i, j, diametro, maiorDiametro=0, maiorDiametroColuna=0, linhaMaior, colunaMaior, saiuDaMoeda=0, media;
	unsigned int hue, tamanhoLinha, tamanhoColuna, quant, inicioLinha, inicioColuna, aceito;
	unsigned char val;

	typedef itk::Image<unsigned char, 2>  ImageType;
	typedef itk::ImageFileReader<ImageType> ReaderType;
	typedef itk::Image<float, 2> FloatImageType;
 

	typedef itk::ImageFileReader<ImageType> ReaderType;
	ReaderType::Pointer reader = ReaderType::New();
	reader->SetFileName (argv[1]);
	reader->Update();


/*------------------------------------------------------------------------------*/
/* Binarização da Imagem                                                        */
/*------------------------------------------------------------------------------*/

	/* Definição para o Threshold */
	int lowerThreshold = 86;
	int upperThreshold = 255;
	
	/* Binary Threshold Image Filter */ 
	typedef itk::BinaryThresholdImageFilter <ImageType, ImageType> BinaryThresholdImageFilterType;
 
	BinaryThresholdImageFilterType::Pointer thresholdFilter = BinaryThresholdImageFilterType::New();
	thresholdFilter->SetInput(reader->GetOutput());
	thresholdFilter->SetLowerThreshold(lowerThreshold);
	thresholdFilter->SetUpperThreshold(upperThreshold);
	thresholdFilter->SetInsideValue(0);
	thresholdFilter->SetOutsideValue(255);
 


/*------------------------------------------------------------------------------*/
/* Melhoramento da Imagem                                                       */
/*------------------------------------------------------------------------------*/

	/* Closing Filter */
	ImageType::Pointer image = thresholdFilter->GetOutput();
 	// thresholdFilter->Update();

	typedef itk::BinaryBallStructuringElement<ImageType::PixelType, ImageType::ImageDimension> StructuringElementType;
	StructuringElementType structuringElement;
	structuringElement.SetRadius(5);
	structuringElement.CreateStructuringElement();

	typedef itk::BinaryMorphologicalClosingImageFilter <ImageType, ImageType, StructuringElementType> BinaryMorphologicalClosingImageFilterType;
	BinaryMorphologicalClosingImageFilterType::Pointer closingFilter = BinaryMorphologicalClosingImageFilterType::New();
	closingFilter->SetInput(image);
	closingFilter->SetKernel(structuringElement);

	closingFilter->Update();


/*------------------------------------------------------------------------------*/
/* Verificação da caixa interna                                                 */
/*------------------------------------------------------------------------------*/


	ImageType::Pointer image2 = closingFilter->GetOutput();
	ImageType::RegionType region2 = image2->GetLargestPossibleRegion();
	ImageType::SizeType size = region2.GetSize();

	MAX_COL = size[0];
	MAX_LIN = size[1];

	itk::ImageRegionConstIterator<ImageType> diameterIterator(image2,region2);

	i = 0;
	hue = 1;
	while((!diameterIterator.IsAtEnd()) && (hue)) {
		j = 0;
		tamanhoLinha = 0;
		while ( (j < MAX_COL) && (hue)) {
			
			val = diameterIterator.Get();

			if(val == 255) { 
				++tamanhoLinha;
			}

			if (tamanhoLinha > 1100) {
				hue = 0;
				linhaMaior = i;
			}

			++diameterIterator;
			++j;
		}

		++i;
	}

	inicioLinha = i+5;
	MAX_LIN = inicioLinha + 1230;




		tamanhoColuna = 0;
		for (j = 0; (j < MAX_LIN); ++j) {
			if(image->GetPixel({j, 0}) == 255)
				++tamanhoColuna; 
			}


		/* Caso a imagem esteja invertida */
		if(tamanhoColuna == MAX_LIN) {

			hue = 1;
			quant = 0;
			
			for (i = (MAX_COL-1); (i > 0) &&  (quant < 6) ; --i) {
				tamanhoColuna = 0;
				for (j = (MAX_LIN-1); (j > 0) && (hue) ; --j) {
					if(image->GetPixel({i, j}) == 255)
						++tamanhoColuna; 

					if(tamanhoColuna > 1100) {
						hue = 0;
						if(quant == 0) {
							maiorDiametroColuna = tamanhoColuna;
							colunaMaior = i;
						}
						++quant;
					}
					else
						quant = 0; 
				} 
			}

			inicioColuna = colunaMaior - 1090;
			MAX_COL = colunaMaior;

		}

		else {

			hue = 1;
			quant = 0;
			
			for (i = 0; (i < MAX_COL) &&  (quant < 6) ; ++i) {
				tamanhoColuna = 0;
				for (j = 0; (j < MAX_LIN) && (hue) ; ++j) {
					if(image->GetPixel({i, j}) == 255)
						++tamanhoColuna; 

					if(tamanhoColuna > 1100) {
						hue = 0;
						if(quant == 0) {
							maiorDiametroColuna = tamanhoColuna;
							colunaMaior = i;
						}
						++quant;

					}
					else
						quant = 0; 
				} 
			}

			inicioColuna = colunaMaior;   
			MAX_COL = colunaMaior + 1090;

		}



/*------------------------------------------------------------------------------*/
/* Encontrando o Diametro                                                       */
/*------------------------------------------------------------------------------*/ 


	colunaMaior = linhaMaior = 0;

	maiorDiametro = 0;
	maiorDiametroColuna = 0;

	aceito = 0;
	while(!aceito) {

		for (i = inicioLinha; i < MAX_LIN ; ++i) {
			diametro = 0;
			for (j = inicioColuna; j < MAX_COL ; ++j) {
				if(image->GetPixel({j, i}) == 0)
					diametro++; 
			}

			if (diametro > maiorDiametro) {
				maiorDiametro = diametro;
				linhaMaior = i;
			}

		}

		if(maiorDiametro < 410)
			aceito = 1;

		if (!aceito) {
			inicioLinha += 1;
			MAX_LIN -= 1;
			inicioColuna += 1;
			MAX_COL -= 1;
			maiorDiametro = 0;
		}

	}


	aceito = 0;
	while(!aceito) {


		for (j = inicioColuna; j < MAX_COL ; ++j) {
			diametro = 0;
			for (i = inicioLinha; i < MAX_LIN; ++i) {
				if(image->GetPixel({j, i}) == 0)
					diametro++; 
			}

			if (diametro > maiorDiametroColuna) {
				maiorDiametroColuna = diametro;
				colunaMaior = i;
			}
			 
		}

		if(maiorDiametroColuna < 410)
			aceito = 1;

		if (!aceito) {
			inicioColuna += 1;
			MAX_COL -= 1;
			inicioLinha += 1;
			MAX_LIN -= 1;
			maiorDiametroColuna = 0;
		}

	}


		media = (maiorDiametro + maiorDiametroColuna)/2;



/*------------------------------------------------------------------------------*/
/* Definindo a Moeda!                                                           */
/*------------------------------------------------------------------------------*/

	puts("");

	// Comparando o tamanho médio encontrado com as faixas de tamanhos
	if (media >= 280 && media <= 319)
		printf("R$ 0,10\n\n");

	else if (media >= 320 && media <= 334)
		printf("R$ 0,05\n\n");

	else if (media >= 335 && media <= 348)
		printf("R$ 0,50\n\n");

	else if (media >= 349 && media <= 354)
		printf("R$ 0,05\n\n");

	else if (media >= 355 && media <= 376)
		printf("R$ 0,25\n\n");
	
	else if (media >= 377 && media <= 410)
		printf("R$ 1,00\n\n");

	else
		printf("Moeda nao identificada :( \n\n");
	


/*------------------------------------------------------------------------------*/
/* Imprimindo a moeda que foi verificada                                        */
/*------------------------------------------------------------------------------*/

	// ImageType::Pointer output = closingFilter->GetOutput();

	// typedef itk::ImageFileWriter<ImageType> WriterType;
	// WriterType::Pointer outputWriter = WriterType::New();
	// outputWriter->SetFileName("output.png");
	// outputWriter->SetInput(output);
	// outputWriter->Update();

 
	return EXIT_SUCCESS;
}

