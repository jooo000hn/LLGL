/*
 * GLRenderTarget.h
 * 
 * This file is part of the "LLGL" project (Copyright (c) 2015 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef LLGL_GL_RENDER_TARGET_H
#define LLGL_GL_RENDER_TARGET_H


#include <LLGL/RenderTarget.h>
#include "GLFramebuffer.h"
#include "GLRenderbuffer.h"
#include "GLTexture.h"
#include <functional>
#include <vector>
#include <memory>


namespace LLGL
{


class GLRenderTarget : public RenderTarget
{

    public:

        GLRenderTarget(const RenderTargetDescriptor& desc);

        void AttachDepthBuffer(const Gs::Vector2ui& size) override;
        void AttachStencilBuffer(const Gs::Vector2ui& size) override;
        void AttachDepthStencilBuffer(const Gs::Vector2ui& size) override;

        void AttachTexture(Texture& texture, const RenderTargetAttachmentDescriptor& attachmentDesc) override;

        void DetachAll() override;

        /* ----- Extended Internal Functions ----- */

        // Blits the multi-sample framebuffer onto the default framebuffer.
        void BlitOntoFrameBuffer();

        // Blits the specified color attachment from the framebuffer onto the screen.
        void BlitOntoScreen(std::size_t colorAttachmentIndex);

        // Returns the active framebuffer (i.e. either the default framebuffer or the multi-sample framebuffer).
        const GLFramebuffer& GetFramebuffer() const;

    private:

        void InitRenderbufferStorage(GLRenderbuffer& renderbuffer, GLenum internalFormat);
        GLenum AttachDefaultRenderbuffer(GLFramebuffer& framebuffer, GLenum attachment);

        void AttachRenderbuffer(const Gs::Vector2ui& size, GLenum internalFormat, GLenum attachment);

        GLenum MakeFramebufferAttachment(GLint internalFormat);

        // Sets the draw buffers for the currently bound FBO.
        void SetDrawBuffers();

        void CheckFramebufferStatus(GLenum status, const std::string& info);

        void CreateOnceFramebufferMS();

        bool HasMultiSampling() const;
        bool HasCustomMultiSampling() const;
        bool HasDepthAttachment() const;

        /* === Members === */

        GLFramebuffer                                   framebuffer_;

        std::unique_ptr<GLRenderbuffer>                 renderbuffer_;

        // Multi-sampled framebuffer; required since we cannot directly draw into a texture when using multi-sampling.
        std::unique_ptr<GLFramebuffer>                  framebufferMS_;

        /*
        For multi-sampled render targets we also need a renderbuffer for each attached texture.
        Otherwise we would need multi-sampled textures (e.g. "glTexImage2DMultisample")
        which is only supported since OpenGL 3.2+, but renderbuffers are supported since OpenGL 3.0+.
        */
        std::vector<std::unique_ptr<GLRenderbuffer>>    renderbuffersMS_;

        std::vector<GLenum>                             colorAttachments_;

        GLsizei                                         multiSamples_           = 0;
        GLbitfield                                      blitMask_               = 0;

};


} // /namespace LLGL


#endif



// ================================================================================
