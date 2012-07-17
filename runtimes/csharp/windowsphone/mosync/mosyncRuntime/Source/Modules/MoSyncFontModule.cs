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
		public class FontInfo
		{
			public FontFamily family;
			public FontWeight weight;
			public FontStyle style;
			public double size = 11;	// SDK default

			public FontInfo Clone()
			{
				FontInfo fi = new FontInfo();
				fi.family = family;
				fi.weight = weight;
				fi.style = style;
				fi.size = size;
				return fi;
			}

			public String GetFullName()
			{
				String name = family.ToString();
				if (weight != FontWeights.Normal)
					name += " " + weight.ToString();
				if (style != FontStyles.Normal)
					name += " " + style.ToString();
				return name;
			}
		};

		private List<FontInfo> mAvailableFonts = new List<FontInfo>();

		private void InitAvailableFonts()
		{
			// I couldn't find a way of enumerating this info. So the list had to be hard coded.
			// SystemTypefaces GlyphTypefaces can't be set as a fontsource. Probably only works with custom ttf/otf fonts.
			mAvailableFonts.Add(new FontInfo() { family = new FontFamily("Arial"), weight = FontWeights.Normal, style = FontStyles.Normal });
			mAvailableFonts.Add(new FontInfo() { family = new FontFamily("Arial"), weight = FontWeights.Black, style = FontStyles.Normal });
			mAvailableFonts.Add(new FontInfo() { family = new FontFamily("Arial"), weight = FontWeights.Bold, style = FontStyles.Normal });
			mAvailableFonts.Add(new FontInfo() { family = new FontFamily("Arial"), weight = FontWeights.Normal, style = FontStyles.Italic });
			mAvailableFonts.Add(new FontInfo() { family = new FontFamily("Calibri"), weight = FontWeights.Normal, style = FontStyles.Normal });
			mAvailableFonts.Add(new FontInfo() { family = new FontFamily("Calibri"), weight = FontWeights.Bold, style = FontStyles.Normal });
			mAvailableFonts.Add(new FontInfo() { family = new FontFamily("Calibri"), weight = FontWeights.Normal, style = FontStyles.Italic });
			mAvailableFonts.Add(new FontInfo() { family = new FontFamily("Comic Sans MS"), weight = FontWeights.Normal, style = FontStyles.Normal });
			mAvailableFonts.Add(new FontInfo() { family = new FontFamily("Comic Sans MS"), weight = FontWeights.Bold, style = FontStyles.Normal });
			mAvailableFonts.Add(new FontInfo() { family = new FontFamily("Courier New"), weight = FontWeights.Normal, style = FontStyles.Normal });
			mAvailableFonts.Add(new FontInfo() { family = new FontFamily("Courier New"), weight = FontWeights.Bold, style = FontStyles.Normal });
			mAvailableFonts.Add(new FontInfo() { family = new FontFamily("Courier New"), weight = FontWeights.Normal, style = FontStyles.Italic });
			mAvailableFonts.Add(new FontInfo() { family = new FontFamily("Georgia"), weight = FontWeights.Normal, style = FontStyles.Normal });
			mAvailableFonts.Add(new FontInfo() { family = new FontFamily("Georgia"), weight = FontWeights.Bold, style = FontStyles.Normal });
			mAvailableFonts.Add(new FontInfo() { family = new FontFamily("Georgia"), weight = FontWeights.Normal, style = FontStyles.Italic });
			mAvailableFonts.Add(new FontInfo() { family = new FontFamily("Lucida Sans Unicode"), weight = FontWeights.Normal, style = FontStyles.Normal });
			mAvailableFonts.Add(new FontInfo() { family = new FontFamily("Malgun Gothic"), weight = FontWeights.Normal, style = FontStyles.Normal });
			mAvailableFonts.Add(new FontInfo() { family = new FontFamily("Meiryo UI"), weight = FontWeights.Normal, style = FontStyles.Normal });
			mAvailableFonts.Add(new FontInfo() { family = new FontFamily("Microsoft YaHei"), weight = FontWeights.Normal, style = FontStyles.Normal });
			mAvailableFonts.Add(new FontInfo() { family = new FontFamily("Segoe UI"), weight = FontWeights.Normal, style = FontStyles.Normal });
			mAvailableFonts.Add(new FontInfo() { family = new FontFamily("Segoe UI"), weight = FontWeights.Bold, style = FontStyles.Normal });
			mAvailableFonts.Add(new FontInfo() { family = new FontFamily("Segoe WP"), weight = FontWeights.Normal, style = FontStyles.Normal });
			mAvailableFonts.Add(new FontInfo() { family = new FontFamily("Segoe WP"), weight = FontWeights.Black, style = FontStyles.Normal });
			mAvailableFonts.Add(new FontInfo() { family = new FontFamily("Segoe WP"), weight = FontWeights.Bold, style = FontStyles.Normal });
			mAvailableFonts.Add(new FontInfo() { family = new FontFamily("Segoe WP"), weight = FontWeights.Light, style = FontStyles.Normal });
			mAvailableFonts.Add(new FontInfo() { family = new FontFamily("Segoe WP"), weight = FontWeights.SemiBold, style = FontStyles.Normal });
			// semilight??
			//mAvailableFonts.Add(new FontInfo() { family = new FontFamily("Segoe WP"), weight = FontWeights., style = FontStyles.Normal });
			mAvailableFonts.Add(new FontInfo() { family = new FontFamily("Tahoma"), weight = FontWeights.Normal, style = FontStyles.Normal });
			mAvailableFonts.Add(new FontInfo() { family = new FontFamily("Tahoma"), weight = FontWeights.Bold, style = FontStyles.Normal });
			mAvailableFonts.Add(new FontInfo() { family = new FontFamily("Times New Roman"), weight = FontWeights.Normal, style = FontStyles.Normal });
			mAvailableFonts.Add(new FontInfo() { family = new FontFamily("Times New Roman"), weight = FontWeights.Bold, style = FontStyles.Normal });
			mAvailableFonts.Add(new FontInfo() { family = new FontFamily("Times New Roman"), weight = FontWeights.Normal, style = FontStyles.Italic });
			mAvailableFonts.Add(new FontInfo() { family = new FontFamily("Trebuchet MS"), weight = FontWeights.Normal, style = FontStyles.Normal });
			mAvailableFonts.Add(new FontInfo() { family = new FontFamily("Trebuchet MS"), weight = FontWeights.Bold, style = FontStyles.Normal });
			mAvailableFonts.Add(new FontInfo() { family = new FontFamily("Trebuchet MS"), weight = FontWeights.Normal, style = FontStyles.Italic });
			mAvailableFonts.Add(new FontInfo() { family = new FontFamily("Verdana"), weight = FontWeights.Normal, style = FontStyles.Normal });
			mAvailableFonts.Add(new FontInfo() { family = new FontFamily("Verdana"), weight = FontWeights.Bold, style = FontStyles.Normal });
			mAvailableFonts.Add(new FontInfo() { family = new FontFamily("Verdana"), weight = FontWeights.Normal, style = FontStyles.Italic });
			mAvailableFonts.Add(new FontInfo() { family = new FontFamily("Webdings"), weight = FontWeights.Normal, style = FontStyles.Normal });
			mAvailableFonts.Add(new FontInfo() { family = new FontFamily("Wingdings"), weight = FontWeights.Normal, style = FontStyles.Normal });
		}

		//		private List<GlyphTypeface> mFonts = new List<GlyphTypeface>();
		private List<FontInfo> mFonts = new List<FontInfo>();

		int mCurrentFont = -1;

		public FontInfo GetFont(int handle)
		{
			return mFonts[handle];
		}

		public FontInfo GetCurrentFont()
		{
			return mFonts[mCurrentFont];
		}

		public void Init(Ioctls ioctls, Core core, Runtime runtime)
		{
			InitAvailableFonts();

			ioctls.maFontGetCount = delegate()
			{
				return mAvailableFonts.Count;
			};

			ioctls.maFontGetName = delegate(int _index, int _buffer, int _bufferLen)
			{
				if (_index > ioctls.maFontGetCount())
				{
					return MoSync.Constants.RES_FONT_INDEX_OUT_OF_BOUNDS;
				}
				else
				{
					String fontName = mAvailableFonts[_index].GetFullName();

					if (fontName.Length > _bufferLen) return MoSync.Constants.RES_FONT_INSUFFICIENT_BUFFER;
					core.GetDataMemory().WriteStringAtAddress(_buffer, fontName, _bufferLen);
					return MoSync.Constants.RES_FONT_OK;
				}
			};

			ioctls.maFontLoadWithName = delegate(int _postScriptName, int _size)
			{

				String fontName = core.GetDataMemory().ReadStringAtAddress(_postScriptName);

				foreach (FontInfo finfo in mAvailableFonts)
				{
					if (finfo.GetFullName() == fontName)
					{
						FontInfo nfi = finfo.Clone();
						nfi.size = _size;
						mFonts.Add(nfi);
						return mFonts.Count - 1;
					}
				}

				return MoSync.Constants.RES_FONT_NAME_NONEXISTENT;
			};

			ioctls.maFontLoadDefault = delegate(int _type, int _style, int _size)
			{
				string name;
				switch (_type)
				{
					case MoSync.Constants.FONT_TYPE_MONOSPACE:
						name = "Courier New";
						break;
					case MoSync.Constants.FONT_TYPE_SERIF:
						name = "Times New Roman";
						break;
					case MoSync.Constants.FONT_TYPE_SANS_SERIF:
						name = "Segoe WP";
						break;
					default:
						return MoSync.Constants.RES_FONT_NO_TYPE_STYLE_COMBINATION;
				}

				FontWeight w = FontWeights.Normal;
				FontStyle s = FontStyles.Normal;
				if ((_style & MoSync.Constants.FONT_STYLE_BOLD) != 0)
					w = FontWeights.Bold;
				if ((_style & MoSync.Constants.FONT_STYLE_ITALIC) != 0)
					s = FontStyles.Italic;

				mFonts.Add(new FontInfo() {
					family = new FontFamily(name),
					weight = w,
					style = s,
					size = _size,
				});

				return mFonts.Count - 1;
			};

			ioctls.maFontDelete = delegate(int _handle)
			{
				mFonts.RemoveAt(_handle);
				return 0;
			};
		}
	}
}
