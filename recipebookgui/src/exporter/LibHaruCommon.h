#pragma once

#include <hpdf.h>

namespace
{

const QString c_author = "RecipeBook";

const float c_border = 56.69f;

const char* c_Encoding = "WinAnsiEncoding";

void error_handler(HPDF_STATUS error_no, HPDF_STATUS detail_no, void*)
{
    printf ("ERROR: error_no=%04X, detail_no=%u\n",
      (HPDF_UINT)error_no, (HPDF_UINT)detail_no);

    throw QException();
}

enum class FontType
{
    Regular,
    Bold,
    Italic,
    BoldItalic
};

HPDF_Font getFont(HPDF_Doc document, FontType type)
{
    switch(type)
    {
    case FontType::Regular:
        return HPDF_GetFont(document, "Helvetica", c_Encoding);

    case FontType::Bold:
        return HPDF_GetFont(document, "Helvetica-Bold", c_Encoding);

    case FontType::Italic:
        return HPDF_GetFont(document, "Helvetica-Oblique", c_Encoding);

    case FontType::BoldItalic:
        return HPDF_GetFont(document, "Helvetica-BoldOblique", c_Encoding);
    }

    throw QException();
}

QByteArray convertString(QString str)
{
    return str.toLatin1();
}

}
