const std = @import("std");
const eql = @import("std").mem.eql;
const expect = @import("std").testing.expect;

//A few helpful things compiled away at compile time
const PATH = "input.txt";

const Digit = struct { rep: []const u8, val: u8 };

const alphaDigits = [_]Digit{
    Digit{ .rep = "one", .val = 1 },
    Digit{ .rep = "two", .val = 2 },
    Digit{ .rep = "three", .val = 3 },
    Digit{ .rep = "four", .val = 4 },
    Digit{ .rep = "five", .val = 5 },
    Digit{ .rep = "six", .val = 6 },
    Digit{ .rep = "seven", .val = 7 },
    Digit{ .rep = "eight", .val = 8 },
    Digit{ .rep = "nine", .val = 9 },
};

pub fn main() !void {
    //Memory handling done here
    var arena = std.heap.ArenaAllocator.init(std.heap.page_allocator);
    defer arena.deinit();

    const ally = arena.allocator();

    const buf = try std.fs.Dir.readFileAlloc(std.fs.cwd(), ally, PATH, 1024 * 1024);

    const sol = solution(buf);
    std.debug.print("Solution is :{d}\n", .{try sol});
}

fn solution(s: []const u8) !u32 {
    var first: ?u8 = null;
    var last: ?u8 = null;
    var sum: u32 = 0;
    var i: u32 = 0;

    while (s[i] < 32) {
        i += 1;
    }

    while (i < s.len) : (i += 1) {
        switch (s[i]) {
            '1'...'9' => {
                first = first orelse s[i] - '0';
                last = s[i] - '0';
            },
            'o', 't', 'f', 's', 'e', 'n' => {
                for (alphaDigits) |digit| {
                    if (i + digit.rep.len < s.len) {
                        if (eql(u8, s[i .. i + digit.rep.len], digit.rep)) {
                            first = first orelse digit.val;
                            last = digit.val;
                        }
                    }
                }
            },
            '\n' => {
                sum += @as(u32, first.? * 10) + @as(u32, last.?);
                first = null;
                last = null;
            },
            else => {},
        }
    }
    sum += @as(u32, first orelse 0 * 10) + @as(u32, last orelse 0); //Last line result is in memory, just not executed.
    return sum;
}

test "sample data" {
    const data: []const u8 =
        \\two1nine 
        \\eightwothree
        \\abcone2threexyz
        \\xtwone3four
        \\4nineeightseven2
        \\zoneight234
        \\7pqrstsixteen 
    ;
    try expect(try solution(data) == 281);
}
