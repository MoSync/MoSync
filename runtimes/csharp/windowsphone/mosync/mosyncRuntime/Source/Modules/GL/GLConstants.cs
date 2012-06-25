public class GLConstants
{
	/* ClearBufferMask */
	public const int GL_DEPTH_BUFFER_BIT = 0x00000100;
	public const int GL_STENCIL_BUFFER_BIT = 0x00000400;
	public const int GL_COLOR_BUFFER_BIT = 0x00004000;

	/* Boolean */
	public const int GL_FALSE = 0;
	public const int GL_TRUE = 1;

	/* BeginMode */
	public const int GL_POINTS = 0x0000;
	public const int GL_LINES = 0x0001;
	public const int GL_LINE_LOOP = 0x0002;
	public const int GL_LINE_STRIP = 0x0003;
	public const int GL_TRIANGLES = 0x0004;
	public const int GL_TRIANGLE_STRIP = 0x0005;
	public const int GL_TRIANGLE_FAN = 0x0006;

	/* AlphaFunction */
	public const int GL_NEVER = 0x0200;
	public const int GL_LESS = 0x0201;
	public const int GL_EQUAL = 0x0202;
	public const int GL_LEQUAL = 0x0203;
	public const int GL_GREATER = 0x0204;
	public const int GL_NOTEQUAL = 0x0205;
	public const int GL_GEQUAL = 0x0206;
	public const int GL_ALWAYS = 0x0207;

	/* BlendingFactorDest */
	public const int GL_ZERO = 0;
	public const int GL_ONE = 1;
	public const int GL_SRC_COLOR = 0x0300;
	public const int GL_ONE_MINUS_SRC_COLOR = 0x0301;
	public const int GL_SRC_ALPHA = 0x0302;
	public const int GL_ONE_MINUS_SRC_ALPHA = 0x0303;
	public const int GL_DST_ALPHA = 0x0304;
	public const int GL_ONE_MINUS_DST_ALPHA = 0x0305;

	/* BlendingFactorSrc */
	/*      GL_ZERO */
	/*      GL_ONE */
	public const int GL_DST_COLOR = 0x0306;
	public const int GL_ONE_MINUS_DST_COLOR = 0x0307;
	public const int GL_SRC_ALPHA_SATURATE = 0x0308;
	/*      GL_SRC_ALPHA */
	/*      GL_ONE_MINUS_SRC_ALPHA */
	/*      GL_DST_ALPHA */
	/*      GL_ONE_MINUS_DST_ALPHA */

	/* ClipPlaneName */
	public const int GL_CLIP_PLANE0 = 0x3000;
	public const int GL_CLIP_PLANE1 = 0x3001;
	public const int GL_CLIP_PLANE2 = 0x3002;
	public const int GL_CLIP_PLANE3 = 0x3003;
	public const int GL_CLIP_PLANE4 = 0x3004;
	public const int GL_CLIP_PLANE5 = 0x3005;

	/* ColorMaterialFace */
	/*      GL_FRONT_AND_BACK */

	/* ColorMaterialParameter */
	/*      GL_AMBIENT_AND_DIFFUSE */

	/* ColorPointerType */
	/*      GL_UNSIGNED_BYTE */
	/*      GL_FLOAT */
	/*      GL_FIXED */

	/* CullFaceMode */
	public const int GL_FRONT = 0x0404;
	public const int GL_BACK = 0x0405;
	public const int GL_FRONT_AND_BACK = 0x0408;

	/* DepthFunction */
	/*      GL_NEVER */
	/*      GL_LESS */
	/*      GL_EQUAL */
	/*      GL_LEQUAL */
	/*      GL_GREATER */
	/*      GL_NOTEQUAL */
	/*      GL_GEQUAL */
	/*      GL_ALWAYS */

	/* EnableCap */
	public const int GL_FOG = 0x0B60;
	public const int GL_LIGHTING = 0x0B50;
	public const int GL_TEXTURE_2D = 0x0DE1;
	public const int GL_CULL_FACE = 0x0B44;
	public const int GL_ALPHA_TEST = 0x0BC0;
	public const int GL_BLEND = 0x0BE2;
	public const int GL_COLOR_LOGIC_OP = 0x0BF2;
	public const int GL_DITHER = 0x0BD0;
	public const int GL_STENCIL_TEST = 0x0B90;
	public const int GL_DEPTH_TEST = 0x0B71;
	/*      GL_LIGHT0 */
	/*      GL_LIGHT1 */
	/*      GL_LIGHT2 */
	/*      GL_LIGHT3 */
	/*      GL_LIGHT4 */
	/*      GL_LIGHT5 */
	/*      GL_LIGHT6 */
	/*      GL_LIGHT7 */
	public const int GL_POINT_SMOOTH = 0x0B10;
	public const int GL_LINE_SMOOTH = 0x0B20;
	//public const int GL_SCISSOR_TEST                   = 0x0C11;
	public const int GL_COLOR_MATERIAL = 0x0B57;
	public const int GL_NORMALIZE = 0x0BA1;
	public const int GL_RESCALE_NORMAL = 0x803A;
	//public const int GL_POLYGON_OFFSET_FILL            = 0x8037;
	public const int GL_VERTEX_ARRAY = 0x8074;
	public const int GL_NORMAL_ARRAY = 0x8075;
	public const int GL_COLOR_ARRAY = 0x8076;
	public const int GL_TEXTURE_COORD_ARRAY = 0x8078;
	public const int GL_MULTISAMPLE = 0x809D;
	public const int GL_SAMPLE_ALPHA_TO_COVERAGE = 0x809E;
	public const int GL_SAMPLE_ALPHA_TO_ONE = 0x809F;
	public const int GL_SAMPLE_COVERAGE = 0x80A0;

	/* ErrorCode */
	public const int GL_NO_ERROR = 0;
	public const int GL_INVALID_ENUM = 0x0500;
	public const int GL_INVALID_VALUE = 0x0501;
	public const int GL_INVALID_OPERATION = 0x0502;
	public const int GL_STACK_OVERFLOW = 0x0503;
	public const int GL_STACK_UNDERFLOW = 0x0504;
	public const int GL_OUT_OF_MEMORY = 0x0505;

	/* FogMode */
	/*      GL_LINEAR */
	public const int GL_EXP = 0x0800;
	public const int GL_EXP2 = 0x0801;

	/* FogParameter */
	public const int GL_FOG_DENSITY = 0x0B62;
	public const int GL_FOG_START = 0x0B63;
	public const int GL_FOG_END = 0x0B64;
	public const int GL_FOG_MODE = 0x0B65;
	public const int GL_FOG_COLOR = 0x0B66;

	/* FrontFaceDirection */
	public const int GL_CW = 0x0900;
	public const int GL_CCW = 0x0901;

	/* GetPName */
	public const int GL_CURRENT_COLOR = 0x0B00;
	public const int GL_CURRENT_NORMAL = 0x0B02;
	public const int GL_CURRENT_TEXTURE_COORDS = 0x0B03;
	public const int GL_POINT_SIZE = 0x0B11;
	public const int GL_POINT_SIZE_MIN = 0x8126;
	public const int GL_POINT_SIZE_MAX = 0x8127;
	public const int GL_POINT_FADE_THRESHOLD_SIZE = 0x8128;
	public const int GL_POINT_DISTANCE_ATTENUATION = 0x8129;
	public const int GL_SMOOTH_POINT_SIZE_RANGE = 0x0B12;
	public const int GL_LINE_WIDTH = 0x0B21;
	public const int GL_SMOOTH_LINE_WIDTH_RANGE = 0x0B22;
	public const int GL_ALIASED_POINT_SIZE_RANGE = 0x846D;
	public const int GL_ALIASED_LINE_WIDTH_RANGE = 0x846E;
	public const int GL_CULL_FACE_MODE = 0x0B45;
	public const int GL_FRONT_FACE = 0x0B46;
	public const int GL_SHADE_MODEL = 0x0B54;
	public const int GL_DEPTH_RANGE = 0x0B70;
	public const int GL_DEPTH_WRITEMASK = 0x0B72;
	public const int GL_DEPTH_CLEAR_VALUE = 0x0B73;
	public const int GL_DEPTH_FUNC = 0x0B74;
	public const int GL_STENCIL_CLEAR_VALUE = 0x0B91;
	public const int GL_STENCIL_FUNC = 0x0B92;
	public const int GL_STENCIL_VALUE_MASK = 0x0B93;
	public const int GL_STENCIL_FAIL = 0x0B94;
	public const int GL_STENCIL_PASS_DEPTH_FAIL = 0x0B95;
	public const int GL_STENCIL_PASS_DEPTH_PASS = 0x0B96;
	public const int GL_STENCIL_REF = 0x0B97;
	public const int GL_STENCIL_WRITEMASK = 0x0B98;
	public const int GL_MATRIX_MODE = 0x0BA0;
	public const int GL_VIEWPORT = 0x0BA2;
	public const int GL_MODELVIEW_STACK_DEPTH = 0x0BA3;
	public const int GL_PROJECTION_STACK_DEPTH = 0x0BA4;
	public const int GL_TEXTURE_STACK_DEPTH = 0x0BA5;
	public const int GL_MODELVIEW_MATRIX = 0x0BA6;
	public const int GL_PROJECTION_MATRIX = 0x0BA7;
	public const int GL_TEXTURE_MATRIX = 0x0BA8;
	public const int GL_ALPHA_TEST_FUNC = 0x0BC1;
	public const int GL_ALPHA_TEST_REF = 0x0BC2;
	public const int GL_BLEND_DST = 0x0BE0;
	public const int GL_BLEND_SRC = 0x0BE1;
	public const int GL_LOGIC_OP_MODE = 0x0BF0;
	public const int GL_SCISSOR_BOX = 0x0C10;
	public const int GL_SCISSOR_TEST = 0x0C11;
	public const int GL_COLOR_CLEAR_VALUE = 0x0C22;
	public const int GL_COLOR_WRITEMASK = 0x0C23;
	//public const int GL_UNPACK_ALIGNMENT               = 0x0CF5;
	//public const int GL_PACK_ALIGNMENT                 = 0x0D05;
	public const int GL_MAX_LIGHTS = 0x0D31;
	public const int GL_MAX_CLIP_PLANES = 0x0D32;
	public const int GL_MAX_TEXTURE_SIZE = 0x0D33;
	public const int GL_MAX_MODELVIEW_STACK_DEPTH = 0x0D36;
	public const int GL_MAX_PROJECTION_STACK_DEPTH = 0x0D38;
	public const int GL_MAX_TEXTURE_STACK_DEPTH = 0x0D39;
	public const int GL_MAX_VIEWPORT_DIMS = 0x0D3A;
	public const int GL_MAX_TEXTURE_UNITS = 0x84E2;
	public const int GL_SUBPIXEL_BITS = 0x0D50;
	public const int GL_RED_BITS = 0x0D52;
	public const int GL_GREEN_BITS = 0x0D53;
	public const int GL_BLUE_BITS = 0x0D54;
	public const int GL_ALPHA_BITS = 0x0D55;
	public const int GL_DEPTH_BITS = 0x0D56;
	public const int GL_STENCIL_BITS = 0x0D57;
	public const int GL_POLYGON_OFFSET_UNITS = 0x2A00;
	public const int GL_POLYGON_OFFSET_FILL = 0x8037;
	public const int GL_POLYGON_OFFSET_FACTOR = 0x8038;
	public const int GL_TEXTURE_BINDING_2D = 0x8069;
	public const int GL_VERTEX_ARRAY_SIZE = 0x807A;
	public const int GL_VERTEX_ARRAY_TYPE = 0x807B;
	public const int GL_VERTEX_ARRAY_STRIDE = 0x807C;
	public const int GL_NORMAL_ARRAY_TYPE = 0x807E;
	public const int GL_NORMAL_ARRAY_STRIDE = 0x807F;
	public const int GL_COLOR_ARRAY_SIZE = 0x8081;
	public const int GL_COLOR_ARRAY_TYPE = 0x8082;
	public const int GL_COLOR_ARRAY_STRIDE = 0x8083;
	public const int GL_TEXTURE_COORD_ARRAY_SIZE = 0x8088;
	public const int GL_TEXTURE_COORD_ARRAY_TYPE = 0x8089;
	public const int GL_TEXTURE_COORD_ARRAY_STRIDE = 0x808A;
	public const int GL_VERTEX_ARRAY_POINTER = 0x808E;
	public const int GL_NORMAL_ARRAY_POINTER = 0x808F;
	public const int GL_COLOR_ARRAY_POINTER = 0x8090;
	public const int GL_TEXTURE_COORD_ARRAY_POINTER = 0x8092;
	public const int GL_SAMPLE_BUFFERS = 0x80A8;
	public const int GL_SAMPLES = 0x80A9;
	public const int GL_SAMPLE_COVERAGE_VALUE = 0x80AA;
	public const int GL_SAMPLE_COVERAGE_INVERT = 0x80AB;

	/* GetTextureParameter */
	/*      GL_TEXTURE_MAG_FILTER */
	/*      GL_TEXTURE_MIN_FILTER */
	/*      GL_TEXTURE_WRAP_S */
	/*      GL_TEXTURE_WRAP_T */

	public const int GL_NUM_COMPRESSED_TEXTURE_FORMATS = 0x86A2;
	public const int GL_COMPRESSED_TEXTURE_FORMATS = 0x86A3;

	/* HintMode */
	public const int GL_DONT_CARE = 0x1100;
	public const int GL_FASTEST = 0x1101;
	public const int GL_NICEST = 0x1102;

	/* HintTarget */
	public const int GL_PERSPECTIVE_CORRECTION_HINT = 0x0C50;
	public const int GL_POINT_SMOOTH_HINT = 0x0C51;
	public const int GL_LINE_SMOOTH_HINT = 0x0C52;
	public const int GL_FOG_HINT = 0x0C54;
	public const int GL_GENERATE_MIPMAP_HINT = 0x8192;

	/* LightModelParameter */
	public const int GL_LIGHT_MODEL_AMBIENT = 0x0B53;
	public const int GL_LIGHT_MODEL_TWO_SIDE = 0x0B52;

	/* LightParameter */
	public const int GL_AMBIENT = 0x1200;
	public const int GL_DIFFUSE = 0x1201;
	public const int GL_SPECULAR = 0x1202;
	public const int GL_POSITION = 0x1203;
	public const int GL_SPOT_DIRECTION = 0x1204;
	public const int GL_SPOT_EXPONENT = 0x1205;
	public const int GL_SPOT_CUTOFF = 0x1206;
	public const int GL_CONSTANT_ATTENUATION = 0x1207;
	public const int GL_LINEAR_ATTENUATION = 0x1208;
	public const int GL_QUADRATIC_ATTENUATION = 0x1209;

	/* DataType */
	public const int GL_BYTE = 0x1400;
	public const int GL_UNSIGNED_BYTE = 0x1401;
	public const int GL_SHORT = 0x1402;
	public const int GL_UNSIGNED_SHORT = 0x1403;
	public const int GL_FLOAT = 0x1406;
	public const int GL_FIXED = 0x140C;

	/* LogicOp */
	public const int GL_CLEAR = 0x1500;
	public const int GL_AND = 0x1501;
	public const int GL_AND_REVERSE = 0x1502;
	public const int GL_COPY = 0x1503;
	public const int GL_AND_INVERTED = 0x1504;
	public const int GL_NOOP = 0x1505;
	public const int GL_XOR = 0x1506;
	public const int GL_OR = 0x1507;
	public const int GL_NOR = 0x1508;
	public const int GL_EQUIV = 0x1509;
	public const int GL_INVERT = 0x150A;
	public const int GL_OR_REVERSE = 0x150B;
	public const int GL_COPY_INVERTED = 0x150C;
	public const int GL_OR_INVERTED = 0x150D;
	public const int GL_NAND = 0x150E;
	public const int GL_SET = 0x150F;

	/* MaterialFace */
	/*      GL_FRONT_AND_BACK */

	/* MaterialParameter */
	public const int GL_EMISSION = 0x1600;
	public const int GL_SHININESS = 0x1601;
	public const int GL_AMBIENT_AND_DIFFUSE = 0x1602;
	/*      GL_AMBIENT */
	/*      GL_DIFFUSE */
	/*      GL_SPECULAR */

	/* MatrixMode */
	public const int GL_MODELVIEW = 0x1700;
	public const int GL_PROJECTION = 0x1701;
	public const int GL_TEXTURE = 0x1702;

	/* NormalPointerType */
	/*      GL_BYTE */
	/*      GL_SHORT */
	/*      GL_FLOAT */
	/*      GL_FIXED */

	/* PixelFormat */
	public const int GL_ALPHA = 0x1906;
	public const int GL_RGB = 0x1907;
	public const int GL_RGBA = 0x1908;
	public const int GL_LUMINANCE = 0x1909;
	public const int GL_LUMINANCE_ALPHA = 0x190A;

	/* PixelStoreParameter */
	public const int GL_UNPACK_ALIGNMENT = 0x0CF5;
	public const int GL_PACK_ALIGNMENT = 0x0D05;

	/* PixelType */
	/*      GL_UNSIGNED_BYTE */
	public const int GL_UNSIGNED_SHORT_4_4_4_4 = 0x8033;
	public const int GL_UNSIGNED_SHORT_5_5_5_1 = 0x8034;
	public const int GL_UNSIGNED_SHORT_5_6_5 = 0x8363;

	/* ShadingModel */
	public const int GL_FLAT = 0x1D00;
	public const int GL_SMOOTH = 0x1D01;

	/* StencilFunction */
	/*      GL_NEVER */
	/*      GL_LESS */
	/*      GL_EQUAL */
	/*      GL_LEQUAL */
	/*      GL_GREATER */
	/*      GL_NOTEQUAL */
	/*      GL_GEQUAL */
	/*      GL_ALWAYS */

	/* StencilOp */
	/*      GL_ZERO */
	public const int GL_KEEP = 0x1E00;
	public const int GL_REPLACE = 0x1E01;
	public const int GL_INCR = 0x1E02;
	public const int GL_DECR = 0x1E03;
	/*      GL_INVERT */

	/* StringName */
	public const int GL_VENDOR = 0x1F00;
	public const int GL_RENDERER = 0x1F01;
	public const int GL_VERSION = 0x1F02;
	public const int GL_EXTENSIONS = 0x1F03;

	/* TexCoordPointerType */
	/*      GL_SHORT */
	/*      GL_FLOAT */
	/*      GL_FIXED */
	/*      GL_BYTE */

	/* TextureEnvMode */
	public const int GL_MODULATE = 0x2100;
	public const int GL_DECAL = 0x2101;
	/*      GL_BLEND */
	public const int GL_ADD = 0x0104;
	/*      GL_REPLACE */

	/* TextureEnvParameter */
	public const int GL_TEXTURE_ENV_MODE = 0x2200;
	public const int GL_TEXTURE_ENV_COLOR = 0x2201;

	/* TextureEnvTarget */
	public const int GL_TEXTURE_ENV = 0x2300;

	/* TextureMagFilter */
	public const int GL_NEAREST = 0x2600;
	public const int GL_LINEAR = 0x2601;

	/* TextureMinFilter */
	/*      GL_NEAREST */
	/*      GL_LINEAR */
	public const int GL_NEAREST_MIPMAP_NEAREST = 0x2700;
	public const int GL_LINEAR_MIPMAP_NEAREST = 0x2701;
	public const int GL_NEAREST_MIPMAP_LINEAR = 0x2702;
	public const int GL_LINEAR_MIPMAP_LINEAR = 0x2703;

	/* TextureParameterName */
	public const int GL_TEXTURE_MAG_FILTER = 0x2800;
	public const int GL_TEXTURE_MIN_FILTER = 0x2801;
	public const int GL_TEXTURE_WRAP_S = 0x2802;
	public const int GL_TEXTURE_WRAP_T = 0x2803;
	public const int GL_GENERATE_MIPMAP = 0x8191;

	/* TextureTarget */
	/*      GL_TEXTURE_2D */

	/* TextureUnit */
	public const int GL_TEXTURE0 = 0x84C0;
	public const int GL_TEXTURE1 = 0x84C1;
	public const int GL_TEXTURE2 = 0x84C2;
	public const int GL_TEXTURE3 = 0x84C3;
	public const int GL_TEXTURE4 = 0x84C4;
	public const int GL_TEXTURE5 = 0x84C5;
	public const int GL_TEXTURE6 = 0x84C6;
	public const int GL_TEXTURE7 = 0x84C7;
	public const int GL_TEXTURE8 = 0x84C8;
	public const int GL_TEXTURE9 = 0x84C9;
	public const int GL_TEXTURE10 = 0x84CA;
	public const int GL_TEXTURE11 = 0x84CB;
	public const int GL_TEXTURE12 = 0x84CC;
	public const int GL_TEXTURE13 = 0x84CD;
	public const int GL_TEXTURE14 = 0x84CE;
	public const int GL_TEXTURE15 = 0x84CF;
	public const int GL_TEXTURE16 = 0x84D0;
	public const int GL_TEXTURE17 = 0x84D1;
	public const int GL_TEXTURE18 = 0x84D2;
	public const int GL_TEXTURE19 = 0x84D3;
	public const int GL_TEXTURE20 = 0x84D4;
	public const int GL_TEXTURE21 = 0x84D5;
	public const int GL_TEXTURE22 = 0x84D6;
	public const int GL_TEXTURE23 = 0x84D7;
	public const int GL_TEXTURE24 = 0x84D8;
	public const int GL_TEXTURE25 = 0x84D9;
	public const int GL_TEXTURE26 = 0x84DA;
	public const int GL_TEXTURE27 = 0x84DB;
	public const int GL_TEXTURE28 = 0x84DC;
	public const int GL_TEXTURE29 = 0x84DD;
	public const int GL_TEXTURE30 = 0x84DE;
	public const int GL_TEXTURE31 = 0x84DF;
	public const int GL_ACTIVE_TEXTURE = 0x84E0;
	public const int GL_CLIENT_ACTIVE_TEXTURE = 0x84E1;

	/* TextureWrapMode */
	public const int GL_REPEAT = 0x2901;
	public const int GL_CLAMP_TO_EDGE = 0x812F;

	/* VertexPointerType */
	/*      GL_SHORT */
	/*      GL_FLOAT */
	/*      GL_FIXED */
	/*      GL_BYTE */

	/* LightName */
	public const int GL_LIGHT0 = 0x4000;
	public const int GL_LIGHT1 = 0x4001;
	public const int GL_LIGHT2 = 0x4002;
	public const int GL_LIGHT3 = 0x4003;
	public const int GL_LIGHT4 = 0x4004;
	public const int GL_LIGHT5 = 0x4005;
	public const int GL_LIGHT6 = 0x4006;
	public const int GL_LIGHT7 = 0x4007;

	/* Buffer Objects */
	public const int GL_ARRAY_BUFFER = 0x8892;
	public const int GL_ELEMENT_ARRAY_BUFFER = 0x8893;

	public const int GL_ARRAY_BUFFER_BINDING = 0x8894;
	public const int GL_ELEMENT_ARRAY_BUFFER_BINDING = 0x8895;
	public const int GL_VERTEX_ARRAY_BUFFER_BINDING = 0x8896;
	public const int GL_NORMAL_ARRAY_BUFFER_BINDING = 0x8897;
	public const int GL_COLOR_ARRAY_BUFFER_BINDING = 0x8898;
	public const int GL_TEXTURE_COORD_ARRAY_BUFFER_BINDING = 0x889A;

	public const int GL_STATIC_DRAW = 0x88E4;
	public const int GL_DYNAMIC_DRAW = 0x88E8;

	public const int GL_BUFFER_SIZE = 0x8764;
	public const int GL_BUFFER_USAGE = 0x8765;

	/* Texture combine + dot3 */
	public const int GL_SUBTRACT = 0x84E7;
	public const int GL_COMBINE = 0x8570;
	public const int GL_COMBINE_RGB = 0x8571;
	public const int GL_COMBINE_ALPHA = 0x8572;
	public const int GL_RGB_SCALE = 0x8573;
	public const int GL_ADD_SIGNED = 0x8574;
	public const int GL_INTERPOLATE = 0x8575;
	public const int GL_CONSTANT = 0x8576;
	public const int GL_PRIMARY_COLOR = 0x8577;
	public const int GL_PREVIOUS = 0x8578;
	public const int GL_OPERAND0_RGB = 0x8590;
	public const int GL_OPERAND1_RGB = 0x8591;
	public const int GL_OPERAND2_RGB = 0x8592;
	public const int GL_OPERAND0_ALPHA = 0x8598;
	public const int GL_OPERAND1_ALPHA = 0x8599;
	public const int GL_OPERAND2_ALPHA = 0x859A;

	public const int GL_ALPHA_SCALE = 0x0D1C;

	public const int GL_SRC0_RGB = 0x8580;
	public const int GL_SRC1_RGB = 0x8581;
	public const int GL_SRC2_RGB = 0x8582;
	public const int GL_SRC0_ALPHA = 0x8588;
	public const int GL_SRC1_ALPHA = 0x8589;
	public const int GL_SRC2_ALPHA = 0x858A;

	public const int GL_DOT3_RGB = 0x86AE;
	public const int GL_DOT3_RGBA = 0x86AF;
};
