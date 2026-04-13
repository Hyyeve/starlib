#include "resource_manager.hpp"

#include <fstream>

#include "stblib/stb_image.hpp"
#include "stblib/stb_image_write.hpp"
#include "tracy/Tracy.hpp"

namespace starlib
{
    resource_manager::resource_manager(const std::filesystem::path& default_search_path) : default_root(default_search_path)
    {
        add_seach_path(default_search_path, 0);
    }

    void resource_manager::add_seach_path(const std::filesystem::path& path, const u32 priority)
    {
        roots.emplace(u32_max - priority, path);
    }

    void resource_manager::remove_search_path(const std::filesystem::path& path)
    {
        ZoneScoped;
        std::erase_if(roots, [path, this](const auto& item)
        {
            const auto& [key, value] = item;
            const bool is_default_root = path == default_root && key == 0;
            return value == path && !is_default_root;
        });
    }

    bool resource_manager::locate_resource(const resource_id& id, std::filesystem::path& out_path) const
    {
        ZoneScoped;
        for (const std::filesystem::path& root : roots | std::views::values)
        {
            const std::filesystem::path absolute = root / id.relative_path;
            if (std::filesystem::exists(absolute))
            {
                out_path = absolute;
                return true;
            }
        }

        return false;
    }

    std::string resource_manager::text_from_stream(std::istream& stream, const u64 read_pos, const u64 size) const
    {
        ZoneScoped;
        std::string out_text;
        stream.seekg(read_pos);
        if (size == u64_max)
        {
            std::stringstream buffer;
            buffer << stream.rdbuf();
            out_text = buffer.str();
        }
        else
        {
            char* data = static_cast<char*>(malloc(size * sizeof(char)));
            stream.read(data, size);
            out_text = std::string(data);
            free(data);
        }

        return out_text;
    }

    void* resource_manager::bytes_from_stream(std::istream& stream, const u64 read_pos, const u64 size) const
    {
        ZoneScoped;
        u64 malloc_size = size;

        if (size == u64_max)
        {
            stream.seekg(0, std::ios::end);
            malloc_size = stream.tellg();
        }

        void* data = malloc(malloc_size * sizeof(char));
        stream.unsetf(std::ios::skipws);
        stream.seekg(read_pos);
        stream.read(static_cast<char*>(data), malloc_size);
        return data;
    }

    image_resource* resource_manager::image_from_stream(std::istream& stream, const u64 read_pos, const u64 size) const
    {
        ZoneScoped;
        image_resource* result = new image_resource;
        i32 temp_width = -1;
        i32 temp_height = -1;
        i32 temp_channels = -1;

        u64 file_size = size;
        if (size == u64_max)
        {
            stream.seekg(0, std::ios::end);
            file_size = stream.tellg();
        }

        void* mem = bytes_from_stream(stream, read_pos, file_size);

        stbi_set_unpremultiply_on_load(true);
        result->data = stbi_load_from_memory(static_cast<stbi_uc*>(mem), file_size, &temp_width, &temp_height, &temp_channels, 0);
        result->width = temp_width;
        result->height = temp_height;
        result->num_channels = temp_channels;

        free(mem);

        return result;
    }

    hdr_image_resource* resource_manager::hdr_image_from_stream(std::istream& stream, const u64 read_pos, const u64 size) const
    {
        ZoneScoped;
        hdr_image_resource* result = new hdr_image_resource;
        i32 temp_width = -1;
        i32 temp_height = -1;
        i32 temp_channels = -1;

        u64 file_size = size;
        if (size == u64_max)
        {
            stream.seekg(0, std::ios::end);
            file_size = stream.tellg();
        }

        void* mem = bytes_from_stream(stream, read_pos, file_size);

        stbi_set_unpremultiply_on_load(true);
        result->data = stbi_loadf_from_memory(static_cast<stbi_uc*>(mem), file_size, &temp_width, &temp_height, &temp_channels, 0);
        result->width = temp_width;
        result->height = temp_height;
        result->num_channels = temp_channels;

        free(mem);

        return result;
    }

    status resource_manager::create_instream(const resource_id& id, std::ifstream& out_stream, const i32 flags) const
    {
        ZoneScoped;
        std::filesystem::path path;
        if (!locate_resource(id, path)) return {status_type::UNKNOWN, "Couldn't locate resource in search paths"};

        std::ifstream file(path, std::ios::in | flags);

        if (!file.is_open() || file.bad())
        {
            file.close();
            return {status_type::UNEXPECTED, "Unable to open resource file"};
        }

        out_stream = std::move(file);

        return status_type::SUCCESS;
    }

    status resource_manager::load_as_text(const resource_id& id, std::string& out_text) const
    {
        ZoneScoped;
        std::ifstream file;
        status load_status = create_instream(id, file);
        if (load_status.is_error()) return load_status;
        out_text = text_from_stream(file);
        file.close();

        return status_type::SUCCESS;
    }

    status resource_manager::load_as_bytes(const resource_id& id, void*& out_bytes, u64& out_size) const
    {
        ZoneScoped;
        std::ifstream file;
        status load_status = create_instream(id, file, std::ios::binary);
        if (load_status.is_error()) return load_status;
        file.unsetf(std::ios::skipws);
        file.seekg(0, std::ios::end);
        out_size = file.tellg();
        out_bytes = bytes_from_stream(file, 0, out_size);
        file.close();

        return status_type::SUCCESS;
    }

    status resource_manager::load_as_image(const resource_id& id, image_resource*& out_image) const
    {
        ZoneScoped;
        std::ifstream file;
        status load_status = create_instream(id, file, std::ios::binary);
        if (load_status.is_error()) return load_status;
        out_image = image_from_stream(file);
        if (out_image->data == nullptr)
        {
            delete out_image;
            return {status_type::BACKEND_ERROR, "Failed to load image with stb_image"};
        }
        return status_type::SUCCESS;
    }

    status resource_manager::load_as_hdr_image(const resource_id& id, hdr_image_resource*& out_image) const
    {
        ZoneScoped;
        std::ifstream file;
        status load_status = create_instream(id, file, std::ios::binary);
        if (load_status.is_error()) return load_status;
        out_image = hdr_image_from_stream(file);
        if (out_image->data == nullptr)
        {
            delete out_image;
            return {status_type::BACKEND_ERROR, "Failed to load image with stb_image"};
        }
        return status_type::SUCCESS;
    }

    status resource_manager::create_outstream(const resource_id& id, std::ofstream& out_stream, const std::filesystem::path& root, const i32 flags) const
    {
        const std::filesystem::path absolute = root / id.relative_path;
        if (!std::filesystem::exists(absolute.parent_path()) && !std::filesystem::create_directories(absolute.parent_path())) return {status_type::UNEXPECTED, "Failed to create directories for resource path"};
        std::ofstream file = std::ofstream(absolute, std::ios::out | flags);
        if (!file.is_open() || file.bad())
        {
            file.close();
            return {status_type::UNEXPECTED, "Failed to open resource file for writing"};
        }

        out_stream = std::move(file);
        return status_type::SUCCESS;
    }

    status resource_manager::create_outstream(const resource_id& id, std::ofstream& out_stream, const i32 flags) const {
        return create_outstream(id, out_stream, roots.begin()->second, flags);
    }

    status resource_manager::write_stream_bytes(std::ostream& stream, const u64 write_pos, const void* data, const u64 size) const
    {
        ZoneScoped;
        stream.unsetf(std::ios::skipws);
        stream.seekp(write_pos);
        stream.write(static_cast<const char*>(data), size * sizeof(char));
        return status_type::SUCCESS;
    }

    status resource_manager::write_stream_text(std::ostream& stream, const u64 write_pos, const std::string_view& text) const
    {
        ZoneScoped;
        stream.seekp(write_pos);
        stream << text;
        return status_type::SUCCESS;
    }


    // ReSharper disable once CppParameterMayBeConstPtrOrRef
    // ReSharper disable once CppParameterMayBeConst
    void stbi_write_callback(void* context, void* data, i32 size)
    {
        std::ostream& file = *static_cast<std::ostream*>(context);
        file.write(static_cast<const char*>(data), size * sizeof(char));
    }

    status resource_manager::write_stream_png(std::ostream& stream, const u64 write_pos, const image_resource& image) const
    {
        ZoneScoped;
        stream.seekp(write_pos);
        const i32 status = stbi_write_png_to_func(stbi_write_callback, &stream, image.width, image.height, image.num_channels, image.data, 0);
        if (!status) return {status_type::BACKEND_ERROR, "stb_image png writing failed"};
        return status_type::SUCCESS;
    }

    status resource_manager::write_stream_jpg(std::ostream& stream, const u64 write_pos, const image_resource& image, const i32 quality) const
    {
        ZoneScoped;
        stream.seekp(write_pos);
        const i32 status = stbi_write_jpg_to_func(stbi_write_callback, &stream, image.width, image.height, image.num_channels, image.data, quality);
        if (!status) return {status_type::BACKEND_ERROR, "stb_image jpg writing failed"};
        return status_type::SUCCESS;
    }
    status resource_manager::write_stream_hdr(std::ostream& stream, const u64 write_pos, const hdr_image_resource& image) const
    {
        ZoneScoped;
        stream.seekp(write_pos);
        const i32 status = stbi_write_hdr_to_func(stbi_write_callback, &stream, image.width, image.height, image.num_channels, image.data);
        if (!status) return {status_type::BACKEND_ERROR, "stb_image hdr writing failed"};
        return status_type::SUCCESS;
    }

    status resource_manager::save_as_bytes(const resource_id& id, const void* data, const u64 size, const std::filesystem::path& root) const
    {
        ZoneScoped;
        std::ofstream file;
        status outstream_status = create_outstream(id, file, root, std::ios::binary | std::ios::trunc);
        if (outstream_status.is_error()) return outstream_status;
        status write_status = write_stream_bytes(file, 0, data, size);
        file.close();
        return write_status;
    }

    status resource_manager::save_as_text(const resource_id& id, const std::string_view& text, const std::filesystem::path& root) const
    {
        ZoneScoped;
        std::ofstream file;
        status outstream_status = create_outstream(id, file, root, std::ios::binary | std::ios::trunc);
        if (outstream_status.is_error()) return outstream_status;
        status write_status =write_stream_text(file, 0, text);
        file.close();
        return write_status;
    }

    status resource_manager::save_as_png(const resource_id& id, const image_resource& image, const std::filesystem::path& root) const
    {
        ZoneScoped;
        std::ofstream file;
        status outstream_status = create_outstream(id, file, root, std::ios::binary | std::ios::trunc);
        if (outstream_status.is_error()) return outstream_status;
        status write_status = write_stream_png(file, 0, image);
        file.close();
        return write_status;
    }

    status resource_manager::save_as_jpg(const resource_id& id, const image_resource& image, const std::filesystem::path& root, const i32 quality) const
    {
        ZoneScoped;
        std::ofstream file;
        status outstream_status = create_outstream(id, file, root, std::ios::binary | std::ios::trunc);
        if (outstream_status.is_error()) return outstream_status;
        status write_status = write_stream_jpg(file, 0, image, quality);
        file.close();
        return write_status;
    }

    status resource_manager::save_as_hdr(const resource_id& id, const hdr_image_resource& image, const std::filesystem::path& root) const
    {
        ZoneScoped;
        std::ofstream file;
        status outstream_status = create_outstream(id, file, root, std::ios::binary | std::ios::trunc);
        if (outstream_status.is_error()) return outstream_status;
        status write_status = write_stream_hdr(file, 0, image);
        file.close();
        return write_status;
    }

    status resource_manager::save_as_bytes(const resource_id& id, const void* data, const u64 size) const
    {
        return save_as_bytes(id, data, size, roots.begin()->second);
    }

    status resource_manager::save_as_text(const resource_id& id, const std::string_view& text) const
    {
        return save_as_text(id, text, roots.begin()->second);
    }

    status resource_manager::save_as_png(const resource_id& id, const image_resource& image_data) const
    {
        return save_as_png(id, image_data, roots.begin()->second);
    }

    status resource_manager::save_as_jpg(const resource_id& id, const image_resource& image_data, const i32 quality) const
    {
        return save_as_jpg(id, image_data, roots.begin()->second, quality);
    }

    status resource_manager::save_as_hdr(const resource_id& id, const hdr_image_resource& image_data) const
    {
        return save_as_hdr(id, image_data, roots.begin()->second);
    }
}
