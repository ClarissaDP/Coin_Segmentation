#include <cstdio>

#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"

//#define MAX_I 1173
//#define MAX_J 1086



int main(int argc, char*argv[]) {

	unsigned int i=0, j, MAX_COL, MAX_LIN, brancos=0, pretos=0, diametro, maiorDiametro=0, maiorDiametroColuna=0, linhaMaior, colunaMaior;

  if(argc < 2)
    {
    std::cerr << "Required: filename" << std::endl;
    return EXIT_FAILURE;
    }


  typedef itk::Image<unsigned char, 2>  ImageType;

  typedef itk::ImageFileReader<ImageType> ReaderType;
  typedef itk::ImageFileWriter<ImageType> WriterType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);
  reader->Update();

  ImageType::Pointer image = reader->GetOutput();
  reader->Update();

  ImageType::RegionType region = image->GetLargestPossibleRegion();

  ImageType::SizeType size =  region.GetSize();

  std::cout << "Input size: " << size << std::endl;


  MAX_COL = size[0];
  MAX_LIN = size[1];
  
  itk::ImageRegionConstIterator<ImageType> imageIterator(image,region);


  while(!imageIterator.IsAtEnd()) 
  {
    j=0;
    diametro=0;
    while (j<MAX_COL){
      // Get the value of the current pixel
      unsigned char val = imageIterator.Get();

 	    //if(val == 255)
 		   //brancos++;

 	    if(val == 0){
 		   //pretos++;
       diametro++;
      }
    
            
      ++imageIterator;
      j++;
    }
    if (diametro>maiorDiametro){
      maiorDiametro=diametro;
      linhaMaior = i;
    }
    i++;
  }

  //verificar video http://cursosabertos.c3sl.ufpr.br/aulas/102 0:11:25
 
  for (i = 0; i < MAX_COL; i++){
	diametro = 0;
	for (j = 0; j < MAX_LIN; j++){
		if(image->GetPixel({{i, j}}) == 0)
			diametro++; 
	}
	if (diametro > maiorDiametroColuna){
		maiorDiametroColuna = diametro;
		colunaMaior = i;
	}
  }

  printf ("O diametro da moeda na imagem %s é de %d pixels na linha %d e %d pixels na coluna %d\nDiametro medio = %d\n\n", argv[1], maiorDiametro, linhaMaior, maiorDiametroColuna, colunaMaior, (maiorDiametro+maiorDiametroColuna)/2);
	//printf("Quantidade brancos: %d\n", brancos);
	//printf("Quantidade pretos: %d\n", pretos);
  

  return EXIT_SUCCESS;
}




/*

10 -> 285   - 287 - 283
5 (amar) -> 312 - 315-309
25 -> 358 - 362 - 355
50 -> 328 - 332 - 324 
1 -> 387 - 384 - 390
  -> 381 - 373 - 389
  -> 
  -> 383 - 385 - 382
  -> 381 - 383 - 379  

*/



/* Voting Binary Iterative Hole Filling Image Filter - Parte Externa da Moeda*/ 
/*
  int r = 3;
  int majorityThreshold = 1;
  unsigned int numberOfIterations = 10;

  const unsigned int Dimension = 2;

  typedef unsigned char PixelType;
 
  typedef itk::VotingBinaryIterativeHoleFillingImageFilter< ImageType > FilterType;
  FilterType::InputSizeType radius;
  radius.Fill( r );

  FilterType::Pointer filter = FilterType::New();
  filter->SetInput( thresholdFilter->GetOutput() );
  filter->SetRadius( radius );
  filter->SetMajorityThreshold( majorityThreshold );
  filter->SetBackgroundValue( itk::NumericTraits< PixelType >::Zero );
  filter->SetForegroundValue( itk::NumericTraits< PixelType >::max() );
  filter->SetMaximumNumberOfIterations( numberOfIterations );
*/

  /* Image Region Iterator */
/*
  ImageType::Pointer image = filter->GetOutput();
  filter->Update();

  ImageType::Pointer outImage = ImageType::New();
  outImage->CopyInformation(image);
  outImage->SetRegions(image->GetLargestPossibleRegion());
  outImage->Allocate();

  ImageType::RegionType region = image->GetLargestPossibleRegion();
  ImageType::RegionType outRegion = outImage->GetLargestPossibleRegion();

  itk::ImageRegionConstIterator<ImageType> imageIterator(image,region);
  itk::ImageRegionIterator<ImageType> outputIterator(outImage,outRegion);

  while(!imageIterator.IsAtEnd())
    {
    // Get the value of the current pixel
    unsigned char val = imageIterator.Get();

    val = abs(val-255);
    
    // Set the current pixel to white
    outputIterator.Set(val);
    
    ++imageIterator;
    ++outputIterator;
    }
*/

  /* Voting Binary Iterative Hole Filling Image Filter - Parte Interna da Moeda */
 /*
  r = 3;
  majorityThreshold = 1;
  numberOfIterations = 10;

  radius.Fill( r );

  FilterType::Pointer filterNovo = FilterType::New();
  filterNovo->SetInput( outImage );
  filterNovo->SetRadius( radius );
  filterNovo->SetMajorityThreshold( majorityThreshold );
  filterNovo->SetBackgroundValue( itk::NumericTraits< PixelType >::Zero );
  filterNovo->SetForegroundValue( itk::NumericTraits< PixelType >::max() );
  filterNovo->SetMaximumNumberOfIterations( numberOfIterations );
  filterNovo->Update();
*/


