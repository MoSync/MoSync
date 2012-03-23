/* Copyright (C) 2012 MoSync AB

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License,
version 2, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.
*/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace MoSync
{
	public interface IAudioInstance
	{
		void Play();
		void Stop();
		void Pause();
		void Dispose();

		// if callback is not null, do an async prepare
		void Prepare(Action async);
		void SetPosition(int _millis);
		int GetPosition();
		int GetLength();
		void SetVolume(float vol);
		void SetNumberOfLoops(int numLoops);

		// A seperate thread is started for the audio instances to be able to poll the state
		void Update();
	}
}
