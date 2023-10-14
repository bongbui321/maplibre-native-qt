// Copyright (C) 2023 MapLibre contributors
// Copyright (C) 2019 Mapbox, Inc.

// SPDX-License-Identifier: BSD-2-Clause

#pragma once

#include <mbgl/actor/actor.hpp>
#include <mbgl/actor/actor_ref.hpp>
#include <mbgl/actor/mailbox.hpp>
#include <mbgl/renderer/renderer_observer.hpp>
#include <mbgl/util/run_loop.hpp>

#include <memory>

namespace QMapLibre {

// Forwards RendererObserver signals to the given
// Delegate RendererObserver on the given RunLoop
class RendererObserver : public mbgl::RendererObserver {
public:
    RendererObserver(mbgl::util::RunLoop &mapRunLoop, mbgl::RendererObserver &delegate_)
        : mailbox(std::make_shared<mbgl::Mailbox>(mapRunLoop)),
          delegate(delegate_, mailbox) {}

    ~RendererObserver() { mailbox->close(); }

    void onInvalidate() final { delegate.invoke(&mbgl::RendererObserver::onInvalidate); }

    void onResourceError(std::exception_ptr err) final {
        delegate.invoke(&mbgl::RendererObserver::onResourceError, err);
    }

    void onWillStartRenderingMap() final { delegate.invoke(&mbgl::RendererObserver::onWillStartRenderingMap); }

    void onWillStartRenderingFrame() final { delegate.invoke(&mbgl::RendererObserver::onWillStartRenderingFrame); }

    void onDidFinishRenderingFrame(RenderMode mode, bool repaintNeeded, bool placementChanged) final {
        delegate.invoke(&mbgl::RendererObserver::onDidFinishRenderingFrame, mode, repaintNeeded, placementChanged);
    }

    void onDidFinishRenderingMap() final { delegate.invoke(&mbgl::RendererObserver::onDidFinishRenderingMap); }

private:
    std::shared_ptr<mbgl::Mailbox> mailbox{};
    mbgl::ActorRef<mbgl::RendererObserver> delegate;
};

} // namespace QMapLibre
