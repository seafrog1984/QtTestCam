#ifndef OPENCV_CVX_TEXT_HPP_
#define OPENCV_CVX_TEXT_HPP_

// source from: http://www.opencv.org.cn/forum.php?mod=viewthread&tid=2083&extra=&page=1
// ֧��OpenCV���ĺ�������

#include <ft2build.h>
#include FT_FREETYPE_H

#include <opencv2/opencv.hpp>

class CvxText {
public:
	/**
	* װ���ֿ��ļ�
	*/
	CvxText(const char* freeType);
	virtual ~CvxText();

	/**
	* ��ȡ����.Ŀǰ��Щ�����в�֧��.
	*
	* \param font        ��������, Ŀǰ��֧��
	* \param size        �����С/�հױ���/�������/��ת�Ƕ�
	* \param underline   �»���
	* \param diaphaneity ͸����
	*
	* \sa setFont, restoreFont
	*/
	void getFont(int* type, cv::Scalar* size = nullptr, bool* underline = nullptr, float* diaphaneity = nullptr);

	/**
	* ��������.Ŀǰ��Щ�����в�֧��.
	*
	* \param font        ��������, Ŀǰ��֧��
	* \param size        �����С/�հױ���/�������/��ת�Ƕ�
	* \param underline   �»���
	* \param diaphaneity ͸����
	*
	* \sa getFont, restoreFont
	*/
	void setFont(int* type, cv::Scalar* size = nullptr, bool* underline = nullptr, float* diaphaneity = nullptr);

	/**
	* �ָ�ԭʼ����������.
	*
	* \sa getFont, setFont
	*/
	void restoreFont();

	/**
	* �������(��ɫĬ��Ϊ��ɫ).��������������ַ���ֹͣ.
	*
	* \param img  �����Ӱ��
	* \param text �ı�����
	* \param pos  �ı�λ��
	*
	* \return ���سɹ�������ַ����ȣ�ʧ�ܷ���-1.
	*/
	int putText(cv::Mat& img, const char* text, cv::Point pos);

	/**
	* �������(��ɫĬ��Ϊ��ɫ).��������������ַ���ֹͣ.
	*
	* \param img  �����Ӱ��
	* \param text �ı�����
	* \param pos  �ı�λ��
	*
	* \return ���سɹ�������ַ����ȣ�ʧ�ܷ���-1.
	*/
	int putText(cv::Mat& img, const wchar_t* text, cv::Point pos);

	/**
	* �������.��������������ַ���ֹͣ.
	*
	* \param img   �����Ӱ��
	* \param text  �ı�����
	* \param pos   �ı�λ��
	* \param color �ı���ɫ
	*
	* \return ���سɹ�������ַ����ȣ�ʧ�ܷ���-1.
	*/
	int putText(cv::Mat& img, const char* text, cv::Point pos, cv::Scalar color);

	/**
	* �������.��������������ַ���ֹͣ.
	*
	* \param img   �����Ӱ��
	* \param text  �ı�����
	* \param pos   �ı�λ��
	* \param color �ı���ɫ
	*
	* \return ���سɹ�������ַ����ȣ�ʧ�ܷ���-1.
	*/
	int putText(cv::Mat& img, const wchar_t* text, cv::Point pos, cv::Scalar color);

private:
	// ��ֹcopy
	CvxText& operator=(const CvxText&);
	// �����ǰ�ַ�, ����m_posλ��
	void putWChar(cv::Mat& img, wchar_t wc, cv::Point& pos, cv::Scalar color);

	FT_Library   m_library;   // �ֿ�
	FT_Face      m_face;      // ����

	// Ĭ�ϵ������������
	int         m_fontType;
	cv::Scalar   m_fontSize;
	bool      m_fontUnderline;
	float      m_fontDiaphaneity;
};

#endif // OPENCV_CVX_TEXT_HPP_
