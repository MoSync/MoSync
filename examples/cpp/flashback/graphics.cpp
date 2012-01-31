/* REminiscence - Flashback interpreter
 * Copyright (C) 2005-2007 Gregory Montoir
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include "graphics.h"


void Graphics::setClippingRect(int16 rx, int16 ry, int16 rw, int16 rh) {
	debug(DBG_VIDEO, "Graphics::setClippingRect(%d, %d, %d, %d)", rx, ry, rw, rh);
	_crx = rx;
	_cry = ry;
	_crw = rw;
	_crh = rh;
}

void Graphics::drawPoint(uint8 color, const Point *pt) {
	debug(DBG_VIDEO, "Graphics::drawPoint() col=0x%X x=%d, y=%d", color, pt->x, pt->y);
	if (pt->x >= 0 && pt->x < _crw && pt->y >= 0 && pt->y < _crh) {
		*(_layer + (pt->y + _cry) * 256 + pt->x + _crx) = color;
	}
}

void Graphics::drawLine(uint8 color, const Point *pt1, const Point *pt2) {
	debug(DBG_VIDEO, "Graphics::drawLine()");
	int16 dxincr1 = 1;
	int16 dyincr1 = 1;
	int16 dx = pt2->x - pt1->x;
	if (dx < 0) {
		dxincr1 = -1;
		dx = -dx;
	}
	int16 dy = pt2->y - pt1->y;
	if (dy < 0) {
		dyincr1 = -1;
		dy = -dy;
	}
	int16 dxincr2, dyincr2, delta1, delta2;
	if (dx < dy) {
		dxincr2 = 0;
		dyincr2 = 1;
		delta1 = dx;
		delta2 = dy;
		if (dyincr1 < 0) {
			dyincr2 = -1;
		}
	} else {
		dxincr2 = 1;
		dyincr2 = 0;
		delta1 = dy;
		delta2 = dx;
		if (dxincr1 < 0) {
			dxincr2 = -1;
		}
	}
	Point pt;
	pt.x = pt1->x;
	pt.y = pt1->y;
	int16 octincr1 = delta1 * 2 - delta2 * 2;
	int16 octincr2 = delta1 * 2;
	int16 oct = delta1 * 2 - delta2;
	if (delta2 >= 0) {
		drawPoint(color, &pt);
		while (--delta2 >= 0) {
			if (oct >= 0) {
				pt.x += dxincr1;
				pt.y += dyincr1;
				oct += octincr1;
			} else {
				pt.x += dxincr2;
				pt.y += dyincr2;
				oct += octincr2;
			}
			drawPoint(color, &pt);
		}
	}
}

void Graphics::addEllipseRadius(int16 y, int16 x1, int16 x2) {
	debug(DBG_VIDEO, "Graphics::addEllipseRadius()");
	if (y >= 0 && y <= _crh) {
		y = (y - _areaPoints[0]) * 2;
		if (x1 < 0) {
			x1 = 0;
		}
		if (x2 >= _crw) {
			x2 = _crw - 1;
		}
		_areaPoints[y + 1] = x1;
		_areaPoints[y + 2] = x2;
	}
}

void Graphics::drawEllipse(uint8 color, bool hasAlpha, const Point *pt, int16 rx, int16 ry) {
	debug(DBG_VIDEO, "Graphics::drawEllipse()");
	bool flag = false;
	int16 y = pt->y - ry;
	if (y < 0) {
		y = 0;
	}
	if (y < _crh) {
		if (pt->y + ry >= 0) {
			_areaPoints[0] = y;
			int32 dy = 0;
			int32 rxsq  = rx * rx;
			int32 rxsq2 = rx * rx * 2;
			int32 rxsq4 = rx * rx * 4;
			int32 rysq  = ry * ry;
			int32 rysq2 = ry * ry * 2;
			int32 rysq4 = ry * ry * 4;

			int32 dx = 0;
			int32 b = rx * ((rysq2 & 0xFFFF) + (rysq2 >> 16));
			int32 a = 2 * b;

			int32 ny1, ny2, nx1, nx2;
			ny1 = ny2 = rysq4 / 2 - a + rxsq;
			nx1 = nx2 = rxsq2 - b + rysq;

			while (ny2 < 0) {
				int16 x2 = pt->x + rx;
				int16 x1 = pt->x - rx;
				int16 by = pt->y + dy;
				int16 ty = pt->y - dy;
				if (x1 != x2) {
					addEllipseRadius(by, x1, x2);
					if (ty < by) {
						addEllipseRadius(ty, x1, x2);
					}
				}
				dy += 1;
				dx += rxsq4;
				nx1 = dx;
				if (nx2 < 0) {
					nx2 += nx1 + rxsq2;
					ny2 += nx1;
				} else {
					--rx;
					a -= rysq4;
					ny1 = a;
					nx2 += nx1 + rxsq2 - ny1;
					ny2 += nx1 + rysq2 - ny1;
				}
			}

			while (rx >= 0) {
				bool flag2 = false;
				int16 x2 = pt->x + rx;
				int16 x1 = pt->x - rx;
				int16 by = pt->y + dy;
				int16 ty = pt->y - dy;
				if (!flag && x1 != x2) {
					flag2 = true;
					addEllipseRadius(by, x1, x2);
					if (ty < by) {
						addEllipseRadius(ty, x1, x2);
					}
				}
				if (flag2) {
					flag = true;
				}
				--rx;
				a -= rysq4;
				nx1 = a;
				if (ny2 < 0) {
					++dy;
					flag = false;
					dx += rxsq4;
					ny2 += dx - nx1 + rysq2;
					ny1 = dx - nx1 + rysq2;
				} else {
					ny2 += rysq2 - nx1;
					ny1 = rysq2 - nx1;
				}
			}
			if (flag) {
				++dy;
			}

			while (dy <= ry) {
				int16 ty = pt->y - dy;
				int16 by = pt->y + dy;
				if (ty < by) {
					addEllipseRadius(ty, pt->x, pt->x);
				}
				addEllipseRadius(by, pt->x, pt->x);
				++dy;
			}
			y = pt->y + ry + 1;
			if (y > _crh) {
				y = _crh;
			}
			y = (y - _areaPoints[0]) * 2;
			_areaPoints[y + 1] = -1;
			fillArea(color, hasAlpha);
		}
	}
}

void Graphics::fillArea(uint8 color, bool hasAlpha) {
	debug(DBG_VIDEO, "Graphics::fillArea()");
	int16 *pts = _areaPoints;
	uint8 *dst = _layer + (_cry + *pts++) * 256 + _crx;
	int16 x1 = *pts++;
	if (x1 >= 0) {
		if (hasAlpha && color > 0xC7) {
			do {
				int16 x2 = *pts++;
				if (x2 < _crw && x2 >= x1) {
					int len = x2 - x1 + 1;
					for (int i = 0; i < len; ++i) {
						*(dst + x1 + i) |= color & 8; // XXX 0x88
					}
				}
				dst += 256;
				x1 = *pts++;
			} while (x1 >= 0);
		} else {
			do {
				int16 x2 = *pts++;
				if (x2 < _crw && x2 >= x1) {
					int len = x2 - x1 + 1;
					memset(dst + x1, color, len);
				}
				dst += 256;
				x1 = *pts++;
			} while (x1 >= 0);
		}
	}
}

void Graphics::drawSegment(uint8 color, bool hasAlpha, int16 ys, const Point *pts, uint8 numPts) {
	debug(DBG_VIDEO, "Graphics::drawSegment()");
	int16 xmin, xmax, ymin, ymax;
	xmin = xmax = pts[0].x;
	ymin = ymax = pts[0].y;
	for (int i = 1; i < numPts; ++i) {
		int16 x = pts[i].x;
		int16 y = pts[i].y;
		if ((xmin << 16) + ymin > (x << 16) + y) {
			xmin = x;
			ymin = y;
		}
		if ((xmax << 16) + ymax < (x << 16) + y) {
			xmax = x;
			ymax = y;
		}
	}
	if (xmin < 0) {
		xmin = 0;
	}
	if (xmax >= _crw) {
		xmax = _crw - 1;
	}
	_areaPoints[0] = ys;
	_areaPoints[1] = xmin;
	_areaPoints[2] = xmax;
	_areaPoints[3] = -1;
	fillArea(color, hasAlpha);
}

void Graphics::drawPolygonOutline(uint8 color, const Point *pts, uint8 numPts) {
	debug(DBG_VIDEO, "Graphics::drawPolygonOutline()");
	assert(numPts >= 2);
	int i;
	for (i = 0; i < numPts - 1; ++i) {
		drawLine(color, &pts[i], &pts[i + 1]);
	}
	drawLine(color, &pts[i], &pts[0]);
}

static int32 calcPolyStep1(int16 dx, int16 dy) {
	debug(DBG_VIDEO, "Graphics::calcPolyStep1()");
	assert(dy != 0);
	int32 a = dx * 256;
	if ((a >> 16) < dy) {
		a = ((int16)(a / dy)) * 256;
	} else {
		a = ((a / 256) / dy) & 0xFFFF0000;
	}
	return a;
}

static int32 calcPolyStep2(int16 dx, int16 dy) {
	debug(DBG_VIDEO, "Graphics::calcPolyStep2()");
	assert(dy != 0);
	int32 a = dx * 256;
	if ((a >> 16) < dy) {
		a = ((int16)(a / dy)) * 256;
	} else {
		a = ((a / 256) / dy) << 16;
	}
	return a;
}

static void drawPolygonHelper1(int32 &x, int16 &y, int32 &step, int16 *&pts, int16 *&start) {
	bool first = true;
	x = pts[0];
	y = pts[1];
	int16 dy, dx;
	do {
		if (first) {
			first = false;
		} else {
			x = *pts;
		}
		--pts;
		dy = *pts - y;
		--pts;
		dx = *pts - x;
	} while (dy <= 0 && start < pts);
	x <<= 16;
	if (dy > 0) {
		step = calcPolyStep1(dx, dy);
	}
}

static void drawPolygonHelper2(int32 &x, int16 &y, int32 &step, int16 *&pts, int16 *&start) {
	bool first = true;
	x = *start++;
	y = *start++;
	int16 dy, dx;
	do {
		if (first) {
			first = false;
		} else {
			x = *start;
			start += 2;
		}
		dy = start[1] - y;
		dx = start[0] - x;
	} while (dy <= 0 && start < pts);
	x <<= 16;
	if (dy > 0) {
		step = calcPolyStep2(dx, dy);
	}
}

void Graphics::drawPolygon(uint8 color, bool hasAlpha, const Point *pts, uint8 numPts) {
	debug(DBG_VIDEO, "Graphics::drawPolygon()");
	assert(numPts * 4 < 0x100);

	int16 *apts1 = &_areaPoints[0x100];
	int16 *apts2 = &_areaPoints[0x100 + numPts * 2];

	int16 xmin, xmax, ymin, ymax;
	xmin = xmax = pts[0].x;
	ymin = ymax = pts[0].y;

	int16 *spts = apts1;
	*apts1++ = *apts2++ = pts[0].x;
	*apts1++ = *apts2++ = pts[0].y;

	for (int p = 1; p < numPts; ++p) {
		int16 x = pts[p].x;
		int16 y = pts[p].y;
		if (ymin > y) {
			ymin = y;
			spts = apts1;
		}
		if (ymax < y) {
			ymax = y;
		}
		*apts1++ = *apts2++ = x;
		*apts1++ = *apts2++ = y;

		if (xmin > x) {
			xmin = x;
		}
		if (xmax < x) {
			xmax = x;
		}
	}
	int16 *rpts = _areaPoints;
	if (xmax < 0 || xmin >= _crw || ymax < 0 || ymin >= _crh) {
		return;
	}
	if (numPts == 2) {
		drawLine(color, &pts[0], &pts[1]);
		return;
	}
	if (ymax == ymin) {
		drawSegment(color, hasAlpha, ymax, pts, numPts);
		return;
	}
	int16 x, dx, y, dy;
	int32 a, b, d, f;
	int32 xstep1 = 0;
	int32 xstep2 = 0;

	apts1 = &spts[numPts * 2];
	xmax = _crw - 1;
	ymax = _crh - 1;
	int32 l1 = 65536;
	int32 l2 = -65536;
	if (ymin < 0) {
		int16 x0, y0;
		do {
			--apts1;
			y0 = *apts1;
			--apts1;
			x0 = *apts1;
		} while (y0 < 0);
		x = apts1[2];
		y = apts1[3];
		dy = y0 - y;
		dx = x0 - x;
		xstep1 = (dy << 16) | dx;
		assert(dy != 0);
		a = y * dx / dy;
		b = (x - a) << 16;
		d = xstep1 = calcPolyStep1(dx, dy);
		if (d < 0) {
			d = -d;
		}
		if (d < l1) {
			d = l2;
		}
		d /= 2;
		b -= d;

		do {
			x0 = *spts++;
			y0 = *spts++;
		} while (*(spts + 1) < 0);
		dy = spts[1] - y0;
		dx = spts[0] - x0;
		xstep2 = (dy << 16) | dx;
		assert(dy != 0);
		a = y0 * dx / dy;
		f = (x0 - a) << 16;
		d = xstep2 = calcPolyStep2(dx, dy);
		if (d < 0) {
			d = -d;
		}
		if (d < l1) {
			d = l1;
		}
		d /= 2;
		f += d;
		ymin = 0;
		*rpts++ = 0;
		goto gfx_startLine;
	}

	*rpts++ = ymin;

gfx_startNewLine:
	drawPolygonHelper2(f, ymin, xstep2, apts1, spts);
	if (spts >= apts1) {
		b = apts1[0] << 16;
		dy = apts1[1];
		if (dy <= ymax) goto gfx_endLine;
		goto gfx_fillArea;
	}
	drawPolygonHelper1(b, ymin, xstep1, apts1, spts);
	d = xstep1;
	if (d < 0) {
		if (d >= l2) {
			d = l1;
		}
		d /= 2;
		b += d;
	}
	d = xstep2;
	if (d >= 0) {
		if (d <= l1) {
			d = l1;
		}
		d /= 2;
		f += d;
	}
	d = b;
	if (d < 0) {
		d = 0;
	}
	x = f >> 16;
	if (x > xmax) {
		x = xmax;
	}
	*rpts++ = d >> 16;
	*rpts++ = x;
	++ymin;
	d = xstep1;
	if (d >= 0) {
		if (d <= l1) {
			d = l1;
		}
		d /= 2;
	}
	b += d;
	d = xstep2;
	if (d < 0) {
		if (d >= l2) {
			d = l1;
		}
		d /= 2;
	}
	f += d;

gfx_startLine:
	while (1) {
		dy = apts1[1];
		if (spts >= apts1) {
			break;
		} else if (dy > spts[1]) {
			dy = spts[1];
			if (dy > ymax) {
				goto gfx_drawPolygonEnd;
			}
			dy -= ymin;
			if (dy > 0) {
				--dy;
				do {
					a = b;
					if (a < 0) {
						a = 0;
					}
					x = f >> 16;
					if (x > xmax) {
						x = xmax;
					}
					*rpts++ = a >> 16;
					*rpts++ = x;
					b += xstep1;
					f += xstep2;
					--dy;
				} while (dy >= 0);
			}
			drawPolygonHelper2(f, ymin, xstep2, apts1, spts);
			d = xstep2;
			if (d >= 0) {
				if (d <= l1) {
					d = l1;
				}
				d /= 2;
				f += d;
			} else {
				d = b;
				if (d < 0) {
					d = 0;
				}
				x = f >> 16;
				if (x > xmax) {
					x = xmax;
				}
				*rpts++ = d >> 16;
				*rpts++ = x;
				++ymin;
				d = xstep2;
				if (d >= l2) {
					d = l1;
				}
				d /= 2;
				f += d;
				b += xstep1;
			}
		} else if (dy == spts[1]) {
			if (dy > ymax) goto gfx_drawPolygonEnd;
			dy -= ymin;
			if (dy > 0) {
				--dy;
				do {
					a = b;
					if (a < 0) {
						a = 0;
					}
					x = f >> 16;
					if (x > xmax) {
						x = xmax;
					}
					*rpts++ = a >> 16;
					*rpts++ = x;
					b += xstep1;
					f += xstep2;
					--dy;
				} while (dy >= 0);
			}
			goto gfx_startNewLine;
		} else if (dy > ymax) {
			goto gfx_drawPolygonEnd;
		} else {
			dy -= ymin;
			if (dy > 0) {
				--dy;
				do {
					a = b;
					if (a < 0) {
						a = 0;
					}
					x = f >> 16;
					if (x > xmax) {
						x = xmax;
					}
					*rpts++ = a >> 16;
					*rpts++ = x;
					b += xstep1;
					f += xstep2;
					--dy;
				} while (dy >= 0);
			}
			drawPolygonHelper1(b, ymin, xstep1, apts1, spts);
			d = xstep1;
			if (d < 0) {
				if (d >= l2) {
					d = l1;
				}
				d /= 2;
				b += d;
			} else {
				d = b;
				if (d < 0) {
					d = 0;
				}
				x = f >> 16;
				if (x > xmax) {
					x = xmax;
				}
				*rpts++ = d >> 16;
				*rpts++ = x;
				++ymin;
				d = xstep1;
				if (d <= l1) {
					d = l1;
				}
				d /= 2;
				b += d;
				f += xstep2;
			}
		}
	}

	if (dy > ymax) goto gfx_drawPolygonEnd;
	dy -= ymin;
	if (dy < 0) goto gfx_fillArea;
	if (dy > 0) {
		--dy;
		do {
			a = b;
			if (a < 0) {
				a = 0;
			}
			x = f >> 16;
			if (x > xmax) {
				x = xmax;
			}
			*rpts++ = a >> 16;
			*rpts++ = x;
			b += xstep1;
			f += xstep2;
			--dy;
		} while (dy >= 0);
	}

	b = f = (apts1[0] << 16) | apts1[1];

gfx_endLine:
	d = xstep1;
	if (d >= 0) {
		if (d >= l1) {
			d /= 2;
			b -= d;
		}
	}
	d = xstep2;
	if (d < 0) {
		d /= 2;
		f -= d;
	}
	a = b;
	if (a < 0) {
		a = 0;
	}
	x = f >> 16;
	if (x > xmax) {
		x = xmax;
	}
	*rpts++ = a >> 16;
	*rpts++ = x;
	goto gfx_fillArea;

gfx_drawPolygonEnd:
	dy = ymax - ymin;
	if (dy >= 0) {
		do {
			a = b;
			if (a < 0) {
				a = 0;
			}
			x = f >> 16;
			if (x > xmax) {
				x = xmax;
			}
			*rpts++ = a >> 16;
			*rpts++ = x;
			b += xstep1;
			f += xstep2;
			--dy;
		} while (dy >= 0);
	}

gfx_fillArea:
	*rpts++ = -1;
	fillArea(color, hasAlpha);
}
