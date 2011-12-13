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
using System.Collections.Generic;

namespace MoSync
{
	public class FontModule : IIoctlModule
	{
		private List<GlyphTypeface> mFonts = new List<GlyphTypeface>();
		GlyphTypeface mCurrentFont = null;
		FontSource mCurrentFontSource = null;

		public GlyphTypeface GetCurrentFont()
		{
			return mCurrentFont;
		}

		public FontSource GetCurrentFontSource()
		{
			return mCurrentFontSource;
		}

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
						en.MoveNext();
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

				String fontName = core.GetDataMemory().ReadStringAtAddress(_postScriptName);
				Typeface typeface = null;
				GlyphTypeface glyphTypeface = null;
				MoSync.Util.RunActionOnMainThreadSync(() =>
				{
					int count = Fonts.SystemTypefaces.Count;
					System.Collections.Generic.IEnumerator<Typeface> en = Fonts.SystemTypefaces.GetEnumerator();
					en.MoveNext();
					while (count != 0)
					{
						typeface = en.Current;
						if (typeface.TryGetGlyphTypeface(out glyphTypeface))
						{
							if (glyphTypeface.FontFileName.StartsWith(fontName))
							{
								break;
							}
							glyphTypeface = null;
						}
						en.MoveNext();
						count--;
					}
				});

				if (glyphTypeface == null) return -2;

				mFonts.Add(glyphTypeface);
				return mFonts.Count - 1;
			};

			ioctls.maFontLoadDefault = delegate(int _type, int _style, int _size)
			{

				//RES_FONT_NO_TYPE_STYLE_COMBINATION
				//RES_FONT_INVALID_SIZE

				switch (_type)
				{
					case MoSync.Constants.FONT_TYPE_MONOSPACE:
						break;
					case MoSync.Constants.FONT_TYPE_SERIF:
						break;
					case MoSync.Constants.FONT_TYPE_SANS_SERIF:
						break;
					default:
						return MoSync.Constants.RES_FONT_NO_TYPE_STYLE_COMBINATION;
				}

				return 0;
			};

			ioctls.maFontSetCurrent = delegate(int _font)
			{
				MoSync.Util.RunActionOnMainThreadSync(() =>
				{
					mCurrentFont = mFonts[_font];
					mCurrentFontSource = new System.Windows.Documents.FontSource(mCurrentFont);
					runtime.GetModule<GraphicsModule>().SetCurrentFontSource(mCurrentFontSource);
				});
				return 0;
			};
		}
	}
}
