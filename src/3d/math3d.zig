const math = @import("std").math;

pub fn Point2(comptime T: type) type {
    return struct {
        x: T,
        y: T,

        const Self = @This();

        pub fn init(x: T, y: T) Self {
            return .{ .x = x, .y = y };
        }
    };
}

pub fn Point3(comptime T: type) type {
    return struct {
        x: T,
        y: T,
        z: T,

        const Self = @This();

        pub fn init(x: T, y: T, z: T) Self {
            return .{ .x = x, .y = y, .z = z };
        }
    };
}

pub fn Vec2(comptime T: type) type {
    return struct {
        x: T,
        y: T,

        const Self = @This();

        pub fn init(x: T, y: T) Self {
            return .{ .x = x, .y = y };
        }
        
        pub fn get(self: *Self, pos: u8) T {
            if(pos == 1) {
                return self.x;
            }
            else if(pos == 2) {
                return self.y;
            }
            else if(pos == 3) {
                return self.z;
            }
            // TODO: return error here?
            return 0;
        }

        pub fn set(self: *Self, pos: u8, val: T) void {
            if(pos == 1) {
                self.x = val;
            }
            else if(pos == 2) {
                self.y = val;
            }
            else if(pos == 3) {
                self.z = val;
            }
            // TODO: return error here?
        }
    };
}

pub fn Vec3(comptime T: type) type {
    return struct {
        x: T,
        y: T,
        z: T,

        const Self = @This();

        pub fn init(x: T, y: T, z: T) Self {
            return .{ .x = x, .y = y, .z = z };
        }

        pub fn get(self: *Self, pos: u8) T {
            if(pos == 1) {
                return self.x;
            }
            else if(pos == 2) {
                return self.y;
            }
            else if(pos == 3) {
                return self.z;
            }
            // TODO: return error here?
            return 0;
        }

        pub fn set(self: *Self, pos: u8, val: T) void {
            if(pos == 1) {
                self.x = val;
            }
            else if(pos == 2) {
                self.y = val;
            }
            else if(pos == 3) {
                self.z = val;
            }
            // TODO: return error here?
        }
    };
}



