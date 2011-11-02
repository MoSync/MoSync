using System;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Ink;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Shapes;

namespace MoSync
{
    public class FontModule : IIoctlModule
    {
        public void Init(Ioctls ioctls, Core core, Runtime runtime)
        {
            ioctls.maFontGetCount = delegate()
            {
                int count = 0;
                MoSync.Util.RunActionOnMainThreadSync(() => count = System.Windows.Media.Fonts.SystemTypefaces.Count);
                return count;
            };

            ioctls.maFontGetName = delegate(int _index, int _buffer, int _bufferLen)
            {
                if (_index > ioctls.maFontGetCount())
                {
                    return MoSync.Constants.RES_FONT_INDEX_OUT_OF_BOUNDS;
                }
                else
                {
                    String fontName = "";
                    MoSync.Util.RunActionOnMainThreadSync(() =>
                    {
                        int count = 0;
                        System.Collections.Generic.IEnumerator<Typeface> en = System.Windows.Media.Fonts.SystemTypefaces.GetEnumerator();
                        while (count < _index)
                        {
                            en.MoveNext();
                            count++;
                        }
                        System.Windows.Media.Typeface currentTypeFace = en.Current;
                        System.Windows.Media.GlyphTypeface currentGlyph;
                        currentTypeFace.TryGetGlyphTypeface(out currentGlyph);
                        fontName = currentGlyph.FontFileName;
                        fontName = (fontName.Split('.'))[0];
                    });

                    if (fontName.Length > _bufferLen) return MoSync.Constants.RES_FONT_INSUFFICIENT_BUFFER;
                    core.GetDataMemory().WriteStringAtAddress(_buffer, fontName, _bufferLen);
                    return MoSync.Constants.RES_FONT_OK;
                }
            };

            ioctls.maFontLoadWithName = delegate(int _postScriptName, int _size)
            {
                return 1;
            };
        }

    }
}
