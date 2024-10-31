
const math = @import("std").math;

pub const Color = struct {
    r: f32,
    g: f32,
    b: f32,

    pub fn quantize(self: *Color, val: u8) Color {
        return Color { 
            .r = math.min(math.round(self.r * val), val),
            .g = math.min(math.round(self.g * val), val),
            .b = math.min(math.round(self.b * val), val)
        };
    }

    pub fn mul(self: *Color, s: f32) Color {
        return Color { 
            .r = self.r * s,
            .g = self.g * s,
            .b = self.b * s
        };
    }

    pub fn add(self: *Color, other: *Color) Color {
        return Color {
            .r = self.r + other.r,
            .g = self.g + other.g,
            .b = self.b + other.b
        };
    }
};

