#pragma once
#include "irrTypes.h"
#include "SColor.h"

namespace irr { namespace video
{
	template <typename T>
	struct SFixedPointColor
	{
		typedef SFixedPointColor<T> MyType;

	public:
		enum E_COLOR_CHANNEL { 
			ECC_ALPHA_CHANNEL, ECC_RED_CHANNEL, ECC_GREEN_CHANNEL, ECC_BLUE_CHANNEL 
		};

		enum 
		{
			CHANNEL_BITS = sizeof(T) << 3,
			FRANCTIONAL_BITS = (sizeof(T) << 3) - 8,
		};
		
	public:
		static u8 fixed_point_to_u8(T  value) { return (u8) (value >> FRANCTIONAL_BITS); }
		static T  u8_to_fixed_point(u8 value) { return ((T) value) << FRANCTIONAL_BITS; }
		
	public:
		SFixedPointColor() {}
		SFixedPointColor(T a, T r, T g, T b) { setFrac(a, r, g, b); }
		SFixedPointColor(u32 color) { set(color); }
		SFixedPointColor(const SColor& scolor) { set(scolor.color) ;}

		void setFracChannel(E_COLOR_CHANNEL channel, T value) { channels[channel] = value;}
		void setFracAlpha(T a) { setFracChannel(ECC_ALPHA_CHANNEL,  a); }
		void setFracRed	 (T r) { setFracChannel(ECC_RED_CHANNEL,	r); }
		void setFracGreen(T g) { setFracChannel(ECC_GREEN_CHANNEL,  g); }
		void setFracBlue (T b) { setFracChannel(ECC_BLUE_CHANNEL,	b); }
		void setFrac(T a, T r, T g, T b)
		{
			setFracAlpha(a);
			setFracRed(r);
			setFracGreen(g);
			setFracBlue(b);
		}

		T getFracChannel(E_COLOR_CHANNEL channel) const { return channels[channel];}
		T getFracAlpha() const { return getFracChannel(ECC_ALPHA_CHANNEL); }
		T getFracRed  () const { return getFracChannel(ECC_RED_CHANNEL); 	}
		T getFracGreen() const { return getFracChannel(ECC_GREEN_CHANNEL); }
		T getFracBlue () const { return getFracChannel(ECC_BLUE_CHANNEL); 	}
		void interpolate(const MyType& target, f32 d, E_COLOR_CHANNEL channel)
		{
			T from = getFracChannel(channel);
			T to = target.getFracChannel(channel);
			setFracChannel(channel, (T)(from*d + to*(1-d)));
		}
		
		void interpolate(const MyType& target, f32 d)
		{
			interpolate(target, d, ECC_ALPHA_CHANNEL);
			interpolate(target, d, ECC_RED_CHANNEL);
			interpolate(target, d, ECC_GREEN_CHANNEL);
			interpolate(target, d, ECC_BLUE_CHANNEL);
		}

//// U8 setter and getter		
		void setChannel(E_COLOR_CHANNEL channel, u8 value) 
		{ 
			setFracChannel(channel, u8_to_fixed_point(value)); 
		}
		
		void setAlpha(u8 a) { setChannel(ECC_ALPHA_CHANNEL, a); }
		void setRed	 (u8 r) { setChannel(ECC_RED_CHANNEL,	r); }
		void setGreen(u8 g) { setChannel(ECC_GREEN_CHANNEL, g); }
		void setBlue (u8 b) { setChannel(ECC_BLUE_CHANNEL,	b); }
		void set(u8 a, u8 r, u8 g, u8 b)
		{
			setAlpha(a);
			setRed(r);
			setGreen(g);
			setBlue(b);
		}

		void set(u32 color)
		{
			setFrac(
				u8_to_fixed_point((color & 0xffffffff) >> 24),
				u8_to_fixed_point((color & 0x00ffffff) >> 16),
				u8_to_fixed_point((color & 0x0000ffff) >>  8),
				u8_to_fixed_point((color & 0x000000ff) >>  0));
		}

		u8 getChannel(E_COLOR_CHANNEL channel) const 
		{ 
			return fixed_point_to_u8(getFracChannel(channel));
		}
		
		u8 getAlpha() const { return getChannel(ECC_ALPHA_CHANNEL); }
		u8 getRed  () const { return getChannel(ECC_RED_CHANNEL); 	}
		u8 getGreen() const { return getChannel(ECC_GREEN_CHANNEL); }
		u8 getBlue () const { return getChannel(ECC_BLUE_CHANNEL); 	}
		
		SColor toSColor() const 
		{
			return SColor(getAlpha(), getRed(), getGreen(), getBlue());
		}
		
	private:
		T channels[4];
	};

	typedef SFixedPointColor<u16> SColor16;
	typedef SFixedPointColor<u32> SColor32;
}}

