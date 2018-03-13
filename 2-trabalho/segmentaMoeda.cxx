#include "itkImage.h"
#include "itkSobelEdgeDetectionImageFilter.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkCastImageFilter.h"
#include "itkBinaryDilateImageFilter.h"
#include "itkBinaryBallStructuringElement.h"
#include "itkBinaryErodeImageFilter.h"
#include "itkBinaryThresholdImageFilter.h"
#include "itkBinaryMorphologicalClosingImageFilter.h"



typedef itk::Image<unsigned char, 2>  ImageType;
typedef itk::Image<float, 2>          FloatImageType;

//typedef FloatImageType ImageType;
//static void CreateImage(ImageType::Pointer);


int main(int argc, char *argv[]) {

	ImageType::Pointer image = ImageType::New();
	if( argc < 3 ) 	{
		std::cerr << "Usage: " << std::endl;
		std::cerr << argv[0] << " inputImageFile" << " outputImageFile" << 	std::endl;
		return EXIT_FAILURE;
	}

	typedef itk::ImageFileReader<ImageType> ReaderType;
	ReaderType::Pointer reader = ReaderType::New();
	reader->SetFileName (argv[1]);

	image = reader->GetOutput();    

	/* Sobel */
	typedef itk::SobelEdgeDetectionImageFilter <ImageType, FloatImageType> SobelEdgeDetectionImageFilterType;
	SobelEdgeDetectionImageFilterType::Pointer sobelFilter = SobelEdgeDetectionImageFilterType::New();
	sobelFilter->SetInput(image);

	typedef itk::CastImageFilter< FloatImageType,ImageType > CastFilterType;
	CastFilterType::Pointer castFilter = CastFilterType::New();
	castFilter->SetInput(sobelFilter->GetOutput());

	/* Dilate */
	unsigned int radius = 5;

	typedef itk::BinaryBallStructuringElement<ImageType::PixelType,2> StructuringElementType;
	StructuringElementType structuringElement;
	structuringElement.SetRadius(radius);
	structuringElement.CreateStructuringElement();

	typedef itk::BinaryDilateImageFilter <ImageType, ImageType, StructuringElementType> BinaryDilateImageFilterType;

	BinaryDilateImageFilterType::Pointer dilateFilter = BinaryDilateImageFilterType::New();
	dilateFilter->SetInput(castFilter->GetOutput());
	dilateFilter->SetKernel(structuringElement);

	/* Threshold */
	int lowerThreshold = 86;
  	int upperThreshold = 255;


	typedef itk::BinaryThresholdImageFilter <ImageType, ImageType> BinaryThresholdImageFilterType;
 
	BinaryThresholdImageFilterType::Pointer thresholdFilter = BinaryThresholdImageFilterType::New();
	thresholdFilter->SetInput(dilateFilter->GetOutput());
	thresholdFilter->SetLowerThreshold(lowerThreshold);
	thresholdFilter->SetUpperThreshold(upperThreshold);
	thresholdFilter->SetInsideValue(255);
	thresholdFilter->SetOutsideValue(0);

	/* Erode */
	radius = 1;

	structuringElement.SetRadius(radius);
	structuringElement.CreateStructuringElement();

	typedef itk::BinaryErodeImageFilter <ImageType, ImageType, StructuringElementType> BinaryErodeImageFilterType;

	BinaryErodeImageFilterType::Pointer erodeFilter = BinaryErodeImageFilterType::New();
	erodeFilter->SetInput(thresholdFilter->GetOutput());
	erodeFilter->SetKernel(structuringElement);

	// radius = 10;
	// structuringElement.SetRadius(radius);
	// structuringElement.CreateStructuringElement();

	// typedef itk::BinaryMorphologicalClosingImageFilter <ImageType, ImageType, StructuringElementType> BinaryMorphologicalClosingImageFilterType;
	// BinaryMorphologicalClosingImageFilterType::Pointer closingFilter = BinaryMorphologicalClosingImageFilterType::New();
	// closingFilter->SetInput(thresholdFilter->GetOutput());
	// closingFilter->SetKernel(structuringElement);



	/* Write image */
	ImageType::Pointer output = closingFilter->GetOutput();

	typedef itk::ImageFileWriter<ImageType> WriterType;
	WriterType::Pointer outputWriter = WriterType::New();
	outputWriter->SetFileName(argv[2]);
	outputWriter->SetInput(output);
	outputWriter->Update();


 	return EXIT_SUCCESS;
}